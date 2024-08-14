// Copyright 2024 Richard Skala

#include "UI/SpaceShooterMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"
#include "UI/GameCreditsScreen.h"
#include "UI/GameOverScreen.h"
#include "UI/HighScoreScreen.h"
#include "UI/MainMenuScreen.h"
#include "UI/PauseScreen.h"
#include "UI/PlayerShipSelectScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuController, Warning, All)

FMainMenuPlayClickedDelegateSignature USpaceShooterMenuController::OnMainMenuPlayClicked;
FShipSelectedDelegateSignature USpaceShooterMenuController::OnPlayerShipSelected;
FGameOverSelectShipClickedDelegateSignature USpaceShooterMenuController::OnGameOverSelectShipClicked;
FGameOverPlayAgainClickedDelegateSignature USpaceShooterMenuController::OnGameOverPlayAgainClicked;
FMainMenuCreditsClickedDelegateSignature USpaceShooterMenuController::OnMainMenuCreditsClicked;
FCreditsMenuBackClickedDelegateSignature USpaceShooterMenuController::OnCreditsMenuBackClicked;
FShipSelectBackButtonClickedDelegateSignature USpaceShooterMenuController::OnShipSelectBackClicked;
FMainMenuHighScoreClickedDelegateSignature USpaceShooterMenuController::OnMainMenuHighScoreClicked;
FHighScoreBackButtonClickedDelegateSignature USpaceShooterMenuController::OnHighScoreBackClicked;

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
		ASpaceShooterGameState::OnRequestPauseGame.AddUniqueDynamic(this, &ThisClass::OnRequestPauseGame);
		ASpaceShooterGameState::OnRequestUnpauseGame.AddUniqueDynamic(this, &ThisClass::OnRequestUnpauseGame);

		// Subscribe to all menu delegates
		OnMainMenuPlayClicked.AddUniqueDynamic(this, &ThisClass::MainMenuPlayClicked);
		OnPlayerShipSelected.AddUniqueDynamic(this, &ThisClass::PlayerShipSelected);
		OnGameOverSelectShipClicked.AddUniqueDynamic(this, &ThisClass::GameOverSelectShipClicked);
		OnGameOverPlayAgainClicked.AddUniqueDynamic(this, &ThisClass::GameOverPlayAgainClicked);
		OnMainMenuCreditsClicked.AddUniqueDynamic(this, &ThisClass::MainMenuCreditsClicked);
		OnCreditsMenuBackClicked.AddUniqueDynamic(this, &ThisClass::CreditsMenuBackClicked);
		OnShipSelectBackClicked.AddUniqueDynamic(this, &ThisClass::ShipSelectBackClicked);
		OnMainMenuHighScoreClicked.AddUniqueDynamic(this, &ThisClass::MainMenuHighScoreClicked);
		OnHighScoreBackClicked.AddUniqueDynamic(this, &ThisClass::HighScoreBackClicked);
	}
}

void USpaceShooterMenuController::PostLoad()
{
	Super::PostLoad(); // NOTE: This is never called
}

void USpaceShooterMenuController::StartMainMenu()
{
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

void USpaceShooterMenuController::ForceClosePauseScreen()
{
	ClosePauseScreen();
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

	// Play "good luck" or "welcome back" VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::GoodLuckVOPlayed))
	{
		SelectAndPlayRandomVO(GoodLuckVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::GoodLuckVOPlayed);
	}
	else if (!HasSoundVOBeenPlayed(ESoundVOPlayed::WelcomeBackVOPlayed))
	{
		SelectAndPlayRandomVO(WelcomeBackVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::WelcomeBackVOPlayed);
	}
}

void USpaceShooterMenuController::OnGameplayEnd(int32 FinalScore, int32 SelectedShipSpriteIndex)
{
	CurrentMenuState = EMenuState::GameOver;

	// Create the Game Over screen
	OpenGameOverScreen(FinalScore);
}

void USpaceShooterMenuController::MainMenuPlayClicked()
{
	PlayButtonClickSound();
	CloseMainMenuScreen();
	OpenPlayerShipSelectScreen();
}

void USpaceShooterMenuController::PlayerShipSelected(int32 ShipSpriteIndex)
{
	PlayButtonClickSound();
	ClosePlayerShipSelectScreen();
}

void USpaceShooterMenuController::GameOverSelectShipClicked()
{
	PlayButtonClickSound();
	CloseGameOverScreen();
	OpenPlayerShipSelectScreen();
}

void USpaceShooterMenuController::GameOverPlayAgainClicked()
{
	PlayButtonClickSound();

	// Close the Game Over screen. The GameState will handle starting the game.
	CloseGameOverScreen();
}

void USpaceShooterMenuController::MainMenuCreditsClicked()
{
	PlayButtonClickSound();
	CloseMainMenuScreen();
	OpenCreditsScreen();
}

void USpaceShooterMenuController::CreditsMenuBackClicked()
{
	PlayButtonClickSound();
	CloseCreditsScreen();
	OpenMainMenuScreen();
}

void USpaceShooterMenuController::ShipSelectBackClicked()
{
	PlayButtonClickSound();
	ClosePlayerShipSelectScreen();
	OpenMainMenuScreen();
}

void USpaceShooterMenuController::MainMenuHighScoreClicked()
{
	PlayButtonClickSound();
	CloseMainMenuScreen();
	OpenHighScoreScreen();
}

void USpaceShooterMenuController::HighScoreBackClicked()
{
	PlayButtonClickSound();
	CloseHighScoreScreen();
	OpenMainMenuScreen();
}

void USpaceShooterMenuController::PauseScreenResumeClicked()
{
	PlayButtonClickSound();
	ClosePauseScreen();
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

	// Play title screen VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::TitleVOPlayed))
	{
		SelectAndPlayRandomVO(TitleVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::TitleVOPlayed);
	}
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

	// Play credits screen VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::CreditsVOPlayed))
	{
		SelectAndPlayRandomVO(CreditsVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::CreditsVOPlayed);
	}
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

	// Play "select your ship" VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::SelectShipVOPlayed))
	{
		SelectAndPlayRandomVO(SelectShipVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::SelectShipVOPlayed);
	}
}

void USpaceShooterMenuController::ClosePlayerShipSelectScreen()
{
	CloseScreen(PlayerShipSelectScreen);
	PlayerShipSelectScreen = nullptr;
}

void USpaceShooterMenuController::OpenGameOverScreen(int32 FinalScore)
{
	// Open the Game Over screen and set the Game Over screen score
	GameOverScreen = Cast<UGameOverScreen>(OpenScreen(GameOverScreenClass));
	if (ensure(GameOverScreen != nullptr))
	{
		GameOverScreen->InitGameOverScreen(FinalScore);
	}

	// Play "game over" VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::GameOverVOPlayed))
	{
		SelectAndPlayRandomVO(GameOverVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::GameOverVOPlayed);
	}
}

void USpaceShooterMenuController::CloseGameOverScreen()
{
	CloseScreen(GameOverScreen);
	GameOverScreen = nullptr;
}

void USpaceShooterMenuController::OpenHighScoreScreen()
{
	HighScoreScreen = Cast<UHighScoreScreen>(OpenScreen(HighScoreScreenClass));
	ensure(HighScoreScreen != nullptr);
	
	// Play "High Scores" VO
	if (!HasSoundVOBeenPlayed(ESoundVOPlayed::HighScoresVOPlayed))
	{
		SelectAndPlayRandomVO(HighScoreVOSounds);
		SetSoundVOPlayed(ESoundVOPlayed::HighScoresVOPlayed);
	}
}

void USpaceShooterMenuController::CloseHighScoreScreen()
{
	CloseScreen(HighScoreScreen);
	HighScoreScreen = nullptr;
}

void USpaceShooterMenuController::OpenPauseScreen()
{
	PauseScreen = Cast<UPauseScreen>(OpenScreen(PauseScreenClass));
	ensure(PauseScreen != nullptr);
}

void USpaceShooterMenuController::ClosePauseScreen()
{
	CloseScreen(PauseScreen);
	PauseScreen = nullptr;
}

void USpaceShooterMenuController::SelectAndPlayRandomVO(TArray<TSoftObjectPtr<USoundBase>> SoundVOArray)
{
	if (SoundVOArray.Num() <= 0)
	{
		return;
	}

	int32 RandomIdx = FMath::RandRange(0, SoundVOArray.Num() - 1);
	TSoftObjectPtr SoundVOToPlayPtr = SoundVOArray[RandomIdx];
	//USoundBase* SoundVOToPlay = SoundVOToPlayPtr.Get(); // This will not always be valid! Use LoadSynchronous() instead.
	USoundBase* SoundVOToPlay = SoundVOToPlayPtr.LoadSynchronous();

	// Stop the current VO sound so they don't overlap
	if (CurrentVOSound != nullptr)
	{
		CurrentVOSound->Stop();
		CurrentVOSound = nullptr;
	}
	CurrentVOSound = UGameplayStatics::SpawnSound2D(GetWorld(), SoundVOToPlay);
	//UGameplayStatics::PlaySound2D(GetWorld(), SoundVOToPlay);
}

bool USpaceShooterMenuController::HasSoundVOBeenPlayed(ESoundVOPlayed SoundVOPlayed) const
{
	return (SoundVOPlayedFlags & (uint8)SoundVOPlayed) != 0;
	//return true; // disable VO temp
}

void USpaceShooterMenuController::SetSoundVOPlayed(ESoundVOPlayed SoundVOPlayed)
{
	SoundVOPlayedFlags |= (uint8)SoundVOPlayed;
}

void USpaceShooterMenuController::PlayButtonClickSound()
{
	if (UIButtonClickSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), UIButtonClickSound);
	}
}

void USpaceShooterMenuController::OnRequestPauseGame()
{
	PlayButtonClickSound();
	OpenPauseScreen();
}

void USpaceShooterMenuController::OnRequestUnpauseGame()
{
	PlayButtonClickSound();
	ClosePauseScreen();
}
