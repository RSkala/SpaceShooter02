// Copyright 2024 Richard Skala

#include "SpaceShooterGameState.h"

#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"

#include "EnemyBase.h"
#include "EnemySpawner.h"
#include "PlayerShipPawn.h"
#include "UI/SpaceShooterMenuController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterGameState, Log, All)

// static member initialization
FGameStartedDelegateSignature ASpaceShooterGameState::OnGameStarted;
FGameEndedDelegateSignature ASpaceShooterGameState::OnGameEnded;
FPlayerScoreChangedDelegateSignature ASpaceShooterGameState::OnPlayerScoreChanged;
FPlayerMultiplierChangedDelegateSignature ASpaceShooterGameState::OnPlayerMultiplierChanged;
FHighScoreChangedDelegateSignature ASpaceShooterGameState::OnPlayerHighScoreChanged;

ASpaceShooterGameState::ASpaceShooterGameState()
{
	UE_LOG(LogSpaceShooterGameState, Log, TEXT("ASpaceShooterGameState::ASpaceShooterGameState - %s"), *GetName());
}

void ASpaceShooterGameState::StartGame()
{
	// Get the player ship in the scene. TODO: Add delegate when player spawns
	if(PlayerShipPawn == nullptr)
	{
		// Temp: Get a reference to the player ship to use as spawned enemy target - TODO: Handle on delegate
		PlayerShipPawn = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShipPawn::StaticClass());
		if (PlayerShipPawn == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - PlayerShipPawn not found"), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}

	// Enable the player ship (set visible and allow controlling of the ship)
	if (PlayerShipPawn != nullptr)
	{
		PlayerShipPawn->EnablePlayer();
	}

	// Set game state to "Gameplay"
	ShooterMenuGameState = EShooterMenuGameState::Gameplay;

	// Reset Score and Multiplier
	PlayerScore = 0;
	CurrentScoreMultiplier = 1;

	// Notify all listeners that gameplay has started
	OnGameStarted.Broadcast();

	// Notify player score changes with "default" values
	OnPlayerScoreChanged.Broadcast(0);
	OnPlayerMultiplierChanged.Broadcast(1);
	OnPlayerHighScoreChanged.Broadcast(PlayerHighScore);
}

void ASpaceShooterGameState::EndGame()
{
	UE_LOG(LogSpaceShooterGameState, Log, TEXT("ASpaceShooterGameState::EndGame - %s"), *GetName());
	OnGameEnded.Broadcast();
}

void ASpaceShooterGameState::BeginPlay()
{
	Super::BeginPlay();

	// Get notified when the player's ship spawns and is destroyed
	APlayerShipPawn::OnPlayerShipSpawned.AddUniqueDynamic(this, &ThisClass::OnPlayerShipSpawned);
	APlayerShipPawn::OnPlayerShipDestroyed.AddUniqueDynamic(this, &ThisClass::OnPlayerShipDestroyed);

	// Start game in Main Menu
	ShooterMenuGameState = EShooterMenuGameState::MainMenu;

	// Create the MenuController
	if (ensure(MenuControllerClass != nullptr))
	{
		//MenuController = NewObject<USpaceShooterMenuController>(this);
		MenuController = NewObject<USpaceShooterMenuController>(this, MenuControllerClass);
		if (ensure(MenuController != nullptr))
		{
			MenuController->StartMainMenu();
		}
	}

	// Create enemy spawner
	if (ensure(EnemySpawnerClass != nullptr))
	{
		if (UWorld* World = GetWorld())
		{
			EnemySpawner = World->SpawnActor<AEnemySpawner>(EnemySpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
			EnemySpawner->SetOwner(this);
			EnemySpawner->SetSpawningEnabled(false);
		}
	}

	// Get notified when an enemy dies
	AEnemyBase::OnEnemyDeath.AddUniqueDynamic(this, &ThisClass::OnEnemyDeath);

	// Listen to menu delegates
	USpaceShooterMenuController::OnMainMenuPlayClicked.AddUniqueDynamic(this, &ThisClass::OnMainMenuPlayClicked);
	USpaceShooterMenuController::OnPlayerShipSelected.AddUniqueDynamic(this, &ThisClass::OnPlayerShipSelected);
	USpaceShooterMenuController::OnGameOverSelectShipClicked.AddUniqueDynamic(this, &ThisClass::OnGameOverSelectShipSelected);
	USpaceShooterMenuController::OnGameOverPlayAgainClicked.AddUniqueDynamic(this, &ThisClass::OnGameOverPlayAgainSelected);
}

void ASpaceShooterGameState::OnPlayerShipSpawned(APlayerShipPawn* const InPlayerShipPawn)
{
	PlayerShipPawn = InPlayerShipPawn;
}

void ASpaceShooterGameState::OnPlayerShipDestroyed()
{
	OnGameEnded.Broadcast();
}

void ASpaceShooterGameState::OnEnemyDeath(FVector EnemyDeathPosition, UNiagaraSystem* EnemyDeathEffect, USoundBase* EnemyDeathSound)
{
	int32 ScoreToAdd = EnemyScoreValue * CurrentScoreMultiplier;
	PlayerScore += ScoreToAdd;

	// Notify listeners that the score has been updated
	OnPlayerScoreChanged.Broadcast(PlayerScore);

	// Check if the new score beats the current high score. Set and notify listeners.
	if (PlayerScore > PlayerHighScore)
	{
		PlayerHighScore = PlayerScore;
		OnPlayerHighScoreChanged.Broadcast(PlayerHighScore);
	}
}

void ASpaceShooterGameState::OnMainMenuPlayClicked()
{
	ShooterMenuGameState = EShooterMenuGameState::ShipSelect;
}

void ASpaceShooterGameState::OnPlayerShipSelected(UPaperSprite* SelectedShipSprite)
{
	ShooterMenuGameState = EShooterMenuGameState::Gameplay;
	StartGame();
}

void ASpaceShooterGameState::OnGameOverSelectShipSelected()
{
	ShooterMenuGameState = EShooterMenuGameState::ShipSelect;
}

void ASpaceShooterGameState::OnGameOverPlayAgainSelected()
{
	ShooterMenuGameState = EShooterMenuGameState::Gameplay;
	StartGame();
}

