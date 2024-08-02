// Copyright 2024 Richard Skala

#include "SpaceShooterGameState.h"

#include "Kismet/GameplayStatics.h"

#include "EnemySpawner.h"
#include "PlayerShipPawn.h"
#include "UI/SpaceShooterMenuController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterGameState, Log, All)

// static member initialization
FGameStartedDelegateSignature ASpaceShooterGameState::OnGameStarted;
FGameEndedDelegateSignature ASpaceShooterGameState::OnGameEnded;

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
}

void ASpaceShooterGameState::EndGame()
{
	UE_LOG(LogSpaceShooterGameState, Log, TEXT("ASpaceShooterGameState::EndGame - %s"), *GetName());
}

void ASpaceShooterGameState::BeginPlay()
{
	Super::BeginPlay();

	// Start game in Main Menu
	ShooterMenuGameState = EShooterMenuGameState::MainMenu;

	// Create the MenuController
	if (ensure(MenuControllerClass != nullptr))
	{
		MenuController = NewObject<USpaceShooterMenuController>(this, MenuControllerClass);
		//MenuController = NewObject<USpaceShooterMenuController>(this);
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
}
