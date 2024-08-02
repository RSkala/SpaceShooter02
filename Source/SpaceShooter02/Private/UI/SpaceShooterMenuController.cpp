// Copyright 2024 Richard Skala

#include "UI/SpaceShooterMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"
#include "UI/GameOverScreen.h"
#include "UI/MainMenuScreen.h"
#include "UI/PlayerShipSelectScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuController, Warning, All)

FMainMenuPlayClickedDelegateSignature USpaceShooterMenuController::OnMainMenuPlayClicked;
FShipSelectedDelegateSignature USpaceShooterMenuController::OnPlayerShipSelected;
FGameOverSelectShipClickedDelegateSignature USpaceShooterMenuController::OnGameOverSelectShipClicked;
FGameOverPlayAgainClickedDelegateSignature USpaceShooterMenuController::OnGameOverPlayAgainClicked;

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

		// Subscribe to all menu delegates
		OnMainMenuPlayClicked.AddUniqueDynamic(this, &ThisClass::MainMenuPlayClicked);
		OnPlayerShipSelected.AddUniqueDynamic(this, &ThisClass::PlayerShipSelected);
		OnGameOverSelectShipClicked.AddUniqueDynamic(this, &ThisClass::GameOverSelectShipClicked);
		OnGameOverPlayAgainClicked.AddUniqueDynamic(this, &ThisClass::GameOverPlayAgainClicked);
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
	MainMenuScreen = Cast<UMainMenuScreen>(OpenScreen(MainMenuScreenClass));
	ensure(MainMenuScreen != nullptr);

	// TODO: Set proper "Input Mode"
	//FInputModeGameAndUI InputMode;
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	//InputMode.SetHideCursorDuringCapture(false);
	//PlayerController->SetInputMode(InputMode);
	//PlayerController->SetShowMouseCursor(true);
}

void USpaceShooterMenuController::OnPlayerShipSelectStart()
{
	CurrentMenuState = EMenuState::ShipSelect;
}

void USpaceShooterMenuController::OnGameplayStart()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::OnGameplayStart"));
	CurrentMenuState = EMenuState::Gameplay;

	// Remove any active menu screens from the viewport

	CloseScreen(MainMenuScreen);
	MainMenuScreen = nullptr;

	CloseScreen(PlayerShipSelectScreen);
	PlayerShipSelectScreen = nullptr;

	CloseScreen(GameOverScreen);
	GameOverScreen = nullptr;
}

void USpaceShooterMenuController::OnGameplayEnd()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::OnGameplayEnd"));
	CurrentMenuState = EMenuState::GameOver;

	// Create the Game Over screen
	GameOverScreen = Cast<UGameOverScreen>(OpenScreen(GameOverScreenClass));
	if (ensure(GameOverScreen != nullptr))
	{
		// Set game over screen score
		if (UWorld* World = GetWorld())
		{
			if (ASpaceShooterGameState* GameState = World->GetGameState<ASpaceShooterGameState>())
			{
				GameOverScreen->InitGameOverScreen(GameState->GetPlayerScore()); // MARKED FOR DEATH. This should be handled by delegates.
			}
		}
	}
}

void USpaceShooterMenuController::MainMenuPlayClicked()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::MainMenuPlayClicked"));

	// Close the Main Menu screen
	CloseScreen(MainMenuScreen);
	MainMenuScreen = nullptr;

	// Open the Player Ship Select screen
	PlayerShipSelectScreen = Cast<UPlayerShipSelectScreen>(OpenScreen(PlayerShipSelectScreenClass));
	ensure(PlayerShipSelectScreen != nullptr);
}

void USpaceShooterMenuController::PlayerShipSelected()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::PlayerShipSelected"));
	CloseScreen(PlayerShipSelectScreen);
	PlayerShipSelectScreen = nullptr;
}

void USpaceShooterMenuController::GameOverSelectShipClicked()
{
	// Close the Game Over screen
	CloseScreen(GameOverScreen);
	GameOverScreen = nullptr;

	// Open the Ship Select screen
	PlayerShipSelectScreen = Cast<UPlayerShipSelectScreen>(OpenScreen(PlayerShipSelectScreenClass));
	ensure(PlayerShipSelectScreen != nullptr);
}

void USpaceShooterMenuController::GameOverPlayAgainClicked()
{
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::GameOverPlayAgainClicked"));

	// Close the Game Over screen. The GameState will handle starting the game.
	CloseScreen(GameOverScreen);
	GameOverScreen = nullptr;
}

UUserWidget* USpaceShooterMenuController::OpenScreen(TSubclassOf<class UUserWidget> ScreenClass)
{
	UUserWidget* NewScreen = nullptr;
	if (ensure(ScreenClass != nullptr))
	{
		UWorld* World = GetWorld();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		NewScreen = UWidgetBlueprintLibrary::Create(World, ScreenClass, PlayerController);
		if (NewScreen != nullptr)
		{
			NewScreen->AddToViewport(0);
		}
	}
	return NewScreen;
}

void USpaceShooterMenuController::CloseScreen(UUserWidget* const ScreenToClose)
{
	if (ScreenToClose != nullptr)
	{
		//ScreenToClose->RemoveFromViewport(); // deprecated
		ScreenToClose->RemoveFromParent();
	}
}
