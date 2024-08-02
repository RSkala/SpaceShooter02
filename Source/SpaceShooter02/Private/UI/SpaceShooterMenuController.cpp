// Copyright 2024 Richard Skala

#include "UI/SpaceShooterMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"
#include "UI/GameOverScreen.h"
#include "UI/MainMenuScreen.h"
#include "UI/PlayerShipSelectScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuController, Warning, All)

USpaceShooterMenuController::USpaceShooterMenuController()
{
}

void USpaceShooterMenuController::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Subscribe to the GameState delegates
		ASpaceShooterGameState::OnGameStarted.AddUniqueDynamic(this, &ThisClass::OnGameplayStart);
		ASpaceShooterGameState::OnGameEnded.AddUniqueDynamic(this, &ThisClass::OnGameplayEnd);
	}
}

void USpaceShooterMenuController::PostLoad()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::PostLoad"));
	Super::PostLoad(); // NOTE: This is never called
}

void USpaceShooterMenuController::StartMainMenu()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::StartMainMenu"));
	CurrentMenuState = EMenuState::MainMenu;

	// Create the Main Menu Screen
	if (ensure(MainMenuScreenClass != nullptr))
	{
		UWorld* World = GetWorld();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

		MainMenuScreen = Cast<UMainMenuScreen>(UWidgetBlueprintLibrary::Create(World, MainMenuScreenClass, PlayerController));
		if (ensure(MainMenuScreen != nullptr))
		{
			MainMenuScreen->AddToViewport(0);
		}

		// TODO: Set proper "Input Mode"

		//FInputModeGameAndUI InputMode;
		//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		//InputMode.SetHideCursorDuringCapture(false);
		//PlayerController->SetInputMode(InputMode);
		//PlayerController->SetShowMouseCursor(true);
	}
}

void USpaceShooterMenuController::OnPlayerShipSelectStart()
{
	CurrentMenuState = EMenuState::ShipSelect;
}

void USpaceShooterMenuController::OnGameplayStart()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::OnGameplayStart"));
	CurrentMenuState = EMenuState::Gameplay;

	// Remove the Main Menu or ShipSelectMenu from the viewport
	if (MainMenuScreen != nullptr)
	{
		//MainMenuScreen->RemoveFromViewport(); // deprecated
		MainMenuScreen->RemoveFromParent();
	}
	MainMenuScreen = nullptr;

	if (PlayerShipSelectScreen != nullptr)
	{
		//PlayerShipSelectScreen->RemoveFromViewport(); // deprecated
		PlayerShipSelectScreen->RemoveFromParent();
	}
	PlayerShipSelectScreen = nullptr;

	// TODO: Enable the player HUD
}

void USpaceShooterMenuController::OnGameplayEnd()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::OnGameplayEnd"));
	CurrentMenuState = EMenuState::GameOver;

	// Create the Game Over screen
	if (ensure(GameOverScreenClass != nullptr))
	{
		UWorld* World = GetWorld();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

		GameOverScreen = Cast<UGameOverScreen>(UWidgetBlueprintLibrary::Create(World, GameOverScreenClass, PlayerController));
		if (ensure(GameOverScreen != nullptr))
		{
			GameOverScreen->AddToViewport(0);

			// Set game over screen score
			if (ASpaceShooterGameState* GameState = World->GetGameState<ASpaceShooterGameState>())
			{
				GameOverScreen->InitGameOverScreen(GameState->GetPlayerScore());
			}
		}
	}
}
