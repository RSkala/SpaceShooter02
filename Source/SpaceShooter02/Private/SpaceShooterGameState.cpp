// Copyright 2024 Richard Skala

#include "SpaceShooterGameState.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"
#include "TimerManager.h"

#include "EnemyBase.h"
#include "EnemySpawner.h"
#include "PickupItemSatelliteWeapon.h"
#include "PickupItemScoreMultiplier.h"
#include "PlayerShipPawn.h"
#include "UI/SpaceShooterMenuController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterGameState, Log, All)

// static member initialization
FGameStartedDelegateSignature ASpaceShooterGameState::OnGameStarted;
FGameEndedDelegateSignature ASpaceShooterGameState::OnGameEnded;
FPlayerScoreChangedDelegateSignature ASpaceShooterGameState::OnPlayerScoreChanged;
FPlayerMultiplierChangedDelegateSignature ASpaceShooterGameState::OnPlayerMultiplierChanged;
FHighScoreChangedDelegateSignature ASpaceShooterGameState::OnPlayerHighScoreChanged;
FAddSatelliteWeaponDelegateSignature ASpaceShooterGameState::OnAddSatelliteWeapon;
FPickupItemPercentChanged ASpaceShooterGameState::OnPickupItemPercentChanged;

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
	NumEnemiesDefeated = 0;

	// Notify all listeners that gameplay has started
	OnGameStarted.Broadcast();

	// Notify player score changes with "default" values
	OnPlayerScoreChanged.Broadcast(0);
	OnPlayerMultiplierChanged.Broadcast(1);
	OnPlayerHighScoreChanged.Broadcast(PlayerHighScore);

	// Reset collected multipliers
	//TotalMultipliersCollected = 0;
	//NumMultipliersCollectedForPowerup = 0;
	OnPickupItemPercentChanged.Broadcast(0.0f);
}

void ASpaceShooterGameState::EndGame()
{
	UE_LOG(LogSpaceShooterGameState, Log, TEXT("ASpaceShooterGameState::EndGame - %s"), *GetName());

	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnGameOverTimerTimeout);
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayAfterGameOver, false);
	}
	else
	{
		OnGameEnded.Broadcast();
	}
}

void ASpaceShooterGameState::AddCurrentScoreMultiplier(int32 AmountToAdd)
{
	CurrentScoreMultiplier += AmountToAdd;

	// TODO: Broadcast score multiplier changed
	//OnPlayerMultiplierChanged.Broadcast(CurrentScoreMultiplier);
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

	// Get notified when a score multiplier is picked up
	APickupItemScoreMultiplier::OnScoreMultiplierPickedUp.AddUniqueDynamic(this, &ThisClass::OnScoreMultiplierPickedUp);

	// Get notified when a satellite weapon is picked up. Filter the message through the GameState.
	APickupItemSatelliteWeapon::OnSatelliteWeaponPickedUp.AddUniqueDynamic(this, &ThisClass::OnSatelliteWeaponPickedUp);

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
	EndGame();
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

	// Spawn a score multiplier at enemy death position
	SpawnScoreMultiplierPickup(EnemyDeathPosition);
}

void ASpaceShooterGameState::OnScoreMultiplierPickedUp(int32 ScoreMultiplierValue)
{
	// Play pickup sound
	if (ScoreMultiplierPickupSound != nullptr)
	{
		// Stop the current pickup item sound, if playing
		if (CurrentMultiplierPickupSound != nullptr)
		{
			CurrentMultiplierPickupSound->Stop();
		}
		CurrentMultiplierPickupSound = nullptr;

		// Adjust a random pitch and play the pickup item sounds
		static const float PitchAdjust = 0.1f;
		float SoundPitch = 1.0f + FMath::FRandRange(-PitchAdjust, PitchAdjust);
		CurrentMultiplierPickupSound = UGameplayStatics::SpawnSound2D(GetWorld(), ScoreMultiplierPickupSound, 0.9f, SoundPitch);
	}

	//// Increment values and notify
	//CurrentScoreMultiplier += ScoreMultiplierValue;
	//OnPlayerMultiplierChanged.Broadcast(CurrentScoreMultiplier);

	//TotalMultipliersCollected++;
	//NumMultipliersCollectedForPowerup++;

	//float Percent = (float)NumMultipliersCollectedForPowerup / (float)NumMultipliersNeededForPowerup;
	//OnPickupItemPercentChanged.Broadcast(Percent);

	//if (NumMultipliersCollectedForPowerup >= NumMultipliersNeededForPowerup)
	//{
	//	NumMultipliersCollectedForPowerup = 0;
	//}
}

void ASpaceShooterGameState::OnSatelliteWeaponPickedUp()
{
	OnAddSatelliteWeapon.Broadcast();
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

void ASpaceShooterGameState::SpawnScoreMultiplierPickup(FVector SpawnPosition)
{
	if (ensure(ScoreMultiplierPickupItemClass != nullptr))
	{
		// Use a random chance to determine score multiplier drops
		float RandomChance = FMath::FRandRange(0.0f, 1.0f);
		if (RandomChance <= ScoreMultiplierDropChance)
		{
			if (UWorld* World = GetWorld())
			{
				World->SpawnActor<APickupItemBase>(ScoreMultiplierPickupItemClass, SpawnPosition, FRotator::ZeroRotator);
			}
		}
	}
}

void ASpaceShooterGameState::OnGameOverTimerTimeout()
{
	UE_LOG(LogSpaceShooterGameState, Log, TEXT("ASpaceShooterGameState::OnGameOverTimerTimeout"));
	OnGameEnded.Broadcast();
}

