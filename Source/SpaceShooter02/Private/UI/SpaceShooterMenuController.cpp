// Copyright 2024 Richard Skala

#include "UI/SpaceShooterMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"
#include "UI/GameCreditsScreen.h"
#include "UI/GameOverScreen.h"
#include "UI/MainMenuScreen.h"
#include "UI/PlayerShipSelectScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuController, Warning, All)

FMainMenuPlayClickedDelegateSignature USpaceShooterMenuController::OnMainMenuPlayClicked;
FShipSelectedDelegateSignature USpaceShooterMenuController::OnPlayerShipSelected;
FGameOverSelectShipClickedDelegateSignature USpaceShooterMenuController::OnGameOverSelectShipClicked;
FGameOverPlayAgainClickedDelegateSignature USpaceShooterMenuController::OnGameOverPlayAgainClicked;
FMainMenuCreditsClickedDelegateSignature USpaceShooterMenuController::OnMainMenuCreditsClicked;
FCreditsMenuBackClickedDelegateSignature USpaceShooterMenuController::OnCreditsMenuBackClicked;
FShipSelectBackButtonClickedDelegateSignature USpaceShooterMenuController::OnShipSelectBackClicked;

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
		OnMainMenuCreditsClicked.AddUniqueDynamic(this, &ThisClass::MainMenuCreditsClicked);
		OnCreditsMenuBackClicked.AddUniqueDynamic(this, &ThisClass::CreditsMenuBackClicked);
		OnShipSelectBackClicked.AddUniqueDynamic(this, &ThisClass::ShipSelectBackClicked);
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

	// Create / Open the Main Menu Screen
	OpenMainMenuScreen();

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
	CurrentMenuState = EMenuState::Gameplay;

	// Remove any possible active menu screens from the viewport
	CloseMainMenuScreen();
	ClosePlayerShipSelectScreen();
	CloseGameOverScreen();
	CloseCreditsScreen();
}

void USpaceShooterMenuController::OnGameplayEnd()
{
	CurrentMenuState = EMenuState::GameOver;

	// Create the Game Over screen
	OpenGameOverScreen();
}

void USpaceShooterMenuController::MainMenuPlayClicked()
{
	CloseMainMenuScreen();
	OpenPlayerShipSelectScreen();
}

void USpaceShooterMenuController::PlayerShipSelected(UPaperSprite* SelectedShipSprite)
{
	ClosePlayerShipSelectScreen();
}

void USpaceShooterMenuController::GameOverSelectShipClicked()
{
	CloseGameOverScreen();
	OpenPlayerShipSelectScreen();
}

void USpaceShooterMenuController::GameOverPlayAgainClicked()
{
	// Close the Game Over screen. The GameState will handle starting the game.
	CloseGameOverScreen();
}

void USpaceShooterMenuController::MainMenuCreditsClicked()
{
	CloseMainMenuScreen();
	OpenCreditsScreen();
}

void USpaceShooterMenuController::CreditsMenuBackClicked()
{
	CloseCreditsScreen();
	OpenMainMenuScreen();
}

void USpaceShooterMenuController::ShipSelectBackClicked()
{
	ClosePlayerShipSelectScreen();
	OpenMainMenuScreen();
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

void USpaceShooterMenuController::OpenMainMenuScreen()
{
	MainMenuScreen = Cast<UMainMenuScreen>(OpenScreen(MainMenuScreenClass));
	ensure(MainMenuScreen != nullptr);
}

void USpaceShooterMenuController::CloseMainMenuScreen()
{
	CloseScreen(MainMenuScreen);
	MainMenuScreen = nullptr;
}

void USpaceShooterMenuController::OpenCreditsScreen()
{
	CreditsScreen = Cast<UGameCreditsScreen>(OpenScreen(CreditsScreenClass));
	ensure(CreditsScreen != nullptr);
}

void USpaceShooterMenuController::CloseCreditsScreen()
{
	CloseScreen(CreditsScreen);
	CreditsScreen = nullptr;
}

void USpaceShooterMenuController::OpenPlayerShipSelectScreen()
{
	PlayerShipSelectScreen = Cast<UPlayerShipSelectScreen>(OpenScreen(PlayerShipSelectScreenClass));
	ensure(PlayerShipSelectScreen != nullptr);
}

void USpaceShooterMenuController::ClosePlayerShipSelectScreen()
{
	CloseScreen(PlayerShipSelectScreen);
	PlayerShipSelectScreen = nullptr;
}

void USpaceShooterMenuController::OpenGameOverScreen()
{
	GameOverScreen = Cast<UGameOverScreen>(OpenScreen(GameOverScreenClass));
	ensure(GameOverScreen != nullptr);

	// Set game over screen score
	if (UWorld* World = GetWorld())
	{
		if (ASpaceShooterGameState* GameState = World->GetGameState<ASpaceShooterGameState>())
		{
			GameOverScreen->InitGameOverScreen(GameState->GetPlayerScore()); // MARKED FOR DEATH. This should be handled by delegates.
		}
	}
}

void USpaceShooterMenuController::CloseGameOverScreen()
{
	CloseScreen(GameOverScreen);
	GameOverScreen = nullptr;
}
