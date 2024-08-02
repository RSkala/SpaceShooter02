// Copyright 2024 Richard Skala

#include "SpaceShooterGameState.h"

#include "Kismet/GameplayStatics.h"

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
	// ------------------------------------------------------
	OnGameStarted.Broadcast();

	// ------------------------------------------------------
	if (EnemySpawner != nullptr)
	{
		EnemySpawner->SetSpawningEnabled(true);
	}

	// Get the player ship in the scene. TODO: Add delegate when player spawns
	//if (!PlayerShipPawn.IsValid())
	if(PlayerShipPawn == nullptr)
	{
		// Temp: Get a reference to the player ship to use as spawned enemy target - TODO: Handle on delegate
		PlayerShipPawn = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShipPawn::StaticClass());
		if (PlayerShipPawn == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - PlayerShipPawn not found"), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}

	if (PlayerShipPawn != nullptr)
	{
		PlayerShipPawn->EnablePlayer();
	}

	// Set game state to "Gameplay"
	ShooterMenuGameState = EShooterMenuGameState::Gameplay;

	// Reset Score and Multiplier
	PlayerScore = 0;
	CurrentScoreMultiplier = 1;

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

	// Notify the spawner when an enemy dies
	AEnemyBase::OnEnemyDeath.AddUniqueDynamic(this, &ThisClass::OnEnemyDeath);
}

void ASpaceShooterGameState::OnPlayerShipSpawned(APlayerShipPawn* const InPlayerShipPawn)
{
	PlayerShipPawn = InPlayerShipPawn;
}

void ASpaceShooterGameState::OnPlayerShipDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("ASpaceShooterGameState::OnPlayerShipDestroyed"));

	//ASpaceShooterGameState::OnGameEnded.AddUniqueDynamic(this, &ThisClass::OnGameplayEnd);
	OnGameEnded.Broadcast();
	
	//MenuController = NewObject<USpaceShooterMenuController>(this, MenuControllerClass);
	////MenuController = NewObject<USpaceShooterMenuController>(this);
	//if (ensure(MenuController != nullptr))
	//{
	//	MenuController->StartMainMenu();
	//}
}

void ASpaceShooterGameState::OnEnemyDeath(FVector EnemyDeathPosition, UNiagaraSystem* EnemyDeathEffect)
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
