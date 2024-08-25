// Copyright 2024 Richard Skala

#include "UI/SpaceShooterMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AudioEnums.h"
#include "SpaceShooterGameInstance.h"
#include "SpaceShooterGameState.h"
#include "UI/DataScreen.h"
#include "UI/GameCreditsScreen.h"
#include "UI/GameOverScreen.h"
#include "UI/HighScoreScreen.h"
#include "UI/HowToPlayScreen.h"
#include "UI/MainMenuScreen.h"
#include "UI/OptionsScreen.h"
#include "UI/PauseScreen.h"
#include "UI/PlayerShipSelectScreen.h"
#include "UI/SoundOptionsScreen.h"
#include "UI/StatsScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuController, Warning, All)

FMainMenuPlayClickedDelegateSignature USpaceShooterMenuController::OnMainMenuPlayClicked;
FShipSelectedDelegateSignature USpaceShooterMenuController::OnPlayerShipSelected;

// Game Over screen actions
FGameOverSelectShipClickedDelegateSignature USpaceShooterMenuController::OnGameOverSelectShipClicked;
FGameOverPlayAgainClickedDelegateSignature USpaceShooterMenuController::OnGameOverPlayAgainClicked;
FGameOverMainMenuClickedDelegateSignature USpaceShooterMenuController::OnGameOverMainMenuClicked;

FMainMenuCreditsClickedDelegateSignature USpaceShooterMenuController::OnMainMenuCreditsClicked;
FShipSelectBackButtonClickedDelegateSignature USpaceShooterMenuController::OnShipSelectBackClicked;
FMainMenuHighScoreClickedDelegateSignature USpaceShooterMenuController::OnMainMenuHighScoreClicked;
FHighScoreBackButtonClickedDelegateSignature USpaceShooterMenuController::OnHighScoreBackClicked;
FMainMenuOptionsButtonClickedDelegateSignature USpaceShooterMenuController::OnMainMenuOptionsClicked;

// Options Screen actions
FOptionsScreenHowToPlayButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenHowToPlayClicked;
FOptionsScreenCreditsButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenCreditsClicked;
FOptionsScreenStatsButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenStatsClicked;
FOptionsScreenSoundsButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenSoundsClicked;
FOptionsScreenDataButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenDataClicked;
FOptionsScreenBackButtonClickedDelegateSignature USpaceShooterMenuController::OnOptionsScreenBackClicked;

// How to Play Screen actions
FHowToPlayScreenBackButtonClickedDelegateSignature USpaceShooterMenuController::OnHowToPlayScreenBackClicked;

// Sound Options Screen actions
FSoundScreenMusicSelectClickedDelegateSignature USpaceShooterMenuController::OnSoundScreenMusicSelectClicked;
FSoundScreenSoundEffectClickedDelegateSignature USpaceShooterMenuController::OnSoundScreenSoundEffectClicked;
FSoundScreenVOClickedDelegateSignature USpaceShooterMenuController::OnSoundScreenVOClicked;
FSoundBackClickedDelegateSignature USpaceShooterMenuController::OnSoundScreenBackClicked;

// Data Screen actions
FDataScreenClearScoresClickedDelegateSignature USpaceShooterMenuController::OnDataScreenClearScoresClicked;
FDataScreenClearStatsClickedDelegateSignature USpaceShooterMenuController::OnDataScreenClearStatsClicked;
FDataScreenBackClickedDelegateSignature USpaceShooterMenuController::OnDataScreenBackClicked;

// Credits Screen actions
FCreditsScreenBackButtonClickedDelegateSignature USpaceShooterMenuController::OnCreditsScreenBackClicked;

// Stats Screen actions
FStatsScreenBackButtonClickedDelegateSignature USpaceShooterMenuController::OnStatsScreenBackClicked;

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

		// Game Over screen
		OnGameOverSelectShipClicked.AddUniqueDynamic(this, &ThisClass::GameOverSelectShipClicked);
		OnGameOverPlayAgainClicked.AddUniqueDynamic(this, &ThisClass::GameOverPlayAgainClicked);
		OnGameOverMainMenuClicked.AddUniqueDynamic(this, &ThisClass::GameOverMainMenuClicked);

		OnMainMenuCreditsClicked.AddUniqueDynamic(this, &ThisClass::MainMenuCreditsClicked);
		OnShipSelectBackClicked.AddUniqueDynamic(this, &ThisClass::ShipSelectBackClicked);
		OnMainMenuHighScoreClicked.AddUniqueDynamic(this, &ThisClass::MainMenuHighScoreClicked);
		OnHighScoreBackClicked.AddUniqueDynamic(this, &ThisClass::HighScoreBackClicked);
		OnMainMenuOptionsClicked.AddUniqueDynamic(this, &ThisClass::MainMenuOptionsButtonClicked);

		// Options Screen
		OnOptionsScreenHowToPlayClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenHowToPlayClicked);
		OnOptionsScreenCreditsClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenCreditsClicked);
		OnOptionsScreenStatsClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenStatsClicked);
		OnOptionsScreenSoundsClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenSoundsClicked);
		OnOptionsScreenDataClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenDataClicked);
		OnOptionsScreenBackClicked.AddUniqueDynamic(this, &ThisClass::OptionsScreenBackClicked);

		// How to Play Screen
		OnHowToPlayScreenBackClicked.AddUniqueDynamic(this, &ThisClass::HowToPlayBackClicked);

		// Sound Screen
		OnSoundScreenMusicSelectClicked.AddUniqueDynamic(this, &ThisClass::SoundScreenMusicSelectClicked);
		OnSoundScreenSoundEffectClicked.AddUniqueDynamic(this, &ThisClass::SoundScreenSoundEffectClicked);
		OnSoundScreenVOClicked.AddUniqueDynamic(this, &ThisClass::SoundScreenVOClicked);
		OnSoundScreenBackClicked.AddUniqueDynamic(this, &ThisClass::SoundScreenBackClicked);

		// Data Screen
		OnDataScreenClearScoresClicked.AddUniqueDynamic(this, &ThisClass::DataScreenClearScoresClicked);
		OnDataScreenClearStatsClicked.AddUniqueDynamic(this, &ThisClass::DataScreenClearStatsClicked);
		OnDataScreenBackClicked.AddUniqueDynamic(this, &ThisClass::DataScreenBackClicked);

		// Credits Screen
		OnCreditsScreenBackClicked.AddUniqueDynamic(this, &ThisClass::CreditsScreenBackClicked);

		// Stats Screen
		OnStatsScreenBackClicked.AddUniqueDynamic(this, &ThisClass::StatsScreenBackClicked);
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
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::GoodLuck);
	}
}

void USpaceShooterMenuController::OnGameplayEnd(
	int32 FinalScore,
	int32 SelectedShipSpriteIndex,
	int32 NumEnemiesDefeated,
	int32 NumScoreMultipliersCollected,
	int32 NumEnemiesDefeatedWithBoost,
	int32 NumProjectilesFired,
	int32 CurrentScoreMultiplier,
	float GameplaySessionLength)
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

void USpaceShooterMenuController::GameOverMainMenuClicked()
{
	PlayButtonClickSound();
	CloseGameOverScreen();
	OpenMainMenuScreen();
}

void USpaceShooterMenuController::MainMenuCreditsClicked()
{
	PlayButtonClickSound();
	CloseMainMenuScreen();
	OpenCreditsScreen();
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

void USpaceShooterMenuController::MainMenuOptionsButtonClicked()
{
	PlayButtonClickSound();
	CloseMainMenuScreen();
	OpenOptionsScreen();
}

void USpaceShooterMenuController::OptionsScreenHowToPlayClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenHowToPlayScreen();
}

void USpaceShooterMenuController::OptionsScreenCreditsClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenCreditsScreen();
}

void USpaceShooterMenuController::OptionsScreenStatsClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenStatsScreen();
}

void USpaceShooterMenuController::OptionsScreenSoundsClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenSoundOptionsScreen();
}

void USpaceShooterMenuController::OptionsScreenDataClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenDataScreen();
}

void USpaceShooterMenuController::OptionsScreenBackClicked()
{
	PlayButtonClickSound();
	CloseOptionsScreen();
	OpenMainMenuScreen();

	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->SaveAudioOptionData();
	}
}

void USpaceShooterMenuController::HowToPlayBackClicked()
{
	PlayButtonClickSound();
	CloseHowToPlayScreen();
	OpenOptionsScreen();
}

void USpaceShooterMenuController::SoundScreenMusicSelectClicked()
{
	PlayButtonClickSound();
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::SoundScreenMusicSelectClicked"));
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->OnCycleMusicSelection();
	}
}

void USpaceShooterMenuController::SoundScreenSoundEffectClicked()
{
	PlayButtonClickSound();
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::SoundScreenSoundEffectClicked"));
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->OnCycleSoundEffectOption();
	}
}

void USpaceShooterMenuController::SoundScreenVOClicked()
{
	PlayButtonClickSound();
	UE_LOG(LogMenuController, Log, TEXT("USpaceShooterMenuController::SoundScreenVOClicked"));
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->OnCycleVOOption();
	}
}

void USpaceShooterMenuController::SoundScreenBackClicked()
{
	PlayButtonClickSound();
	CloseSoundOptionsScreen();
	OpenOptionsScreen();
}

void USpaceShooterMenuController::DataScreenClearScoresClicked()
{
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::ShipExplosionSound);
		GameInstance->ClearHighScores();
	}
}

void USpaceShooterMenuController::DataScreenClearStatsClicked()
{
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::ShipExplosionSound);
		GameInstance->ClearStats();
	}
}

void USpaceShooterMenuController::DataScreenBackClicked()
{
	PlayButtonClickSound();
	CloseDataScreen();
	OpenOptionsScreen();
}

void USpaceShooterMenuController::CreditsScreenBackClicked()
{
	PlayButtonClickSound();
	CloseCreditsScreen();
	OpenOptionsScreen();
}

void USpaceShooterMenuController::StatsScreenBackClicked(float TimeSpentLookingAtStats)
{
	PlayButtonClickSound();

	// Save stat
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->SaveTimeSpentLookingAtStats(TimeSpentLookingAtStats);
	}

	CloseStatsScreen();
	OpenOptionsScreen();
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
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::Title);
	}
}

void USpaceShooterMenuController::CloseMainMenuScreen()
{
	CloseScreen(MainMenuScreen);
	MainMenuScreen = nullptr;
}

void USpaceShooterMenuController::OpenHowToPlayScreen()
{
	HowToPlayScreen = Cast<UHowToPlayScreen>(OpenScreen(HowToPlayScreenClass));
	ensure(HowToPlayScreen != nullptr);
}

void USpaceShooterMenuController::CloseHowToPlayScreen()
{
	CloseScreen(HowToPlayScreen);
	HowToPlayScreen = nullptr;
}

void USpaceShooterMenuController::OpenCreditsScreen()
{
	CreditsScreen = Cast<UGameCreditsScreen>(OpenScreen(CreditsScreenClass));
	ensure(CreditsScreen != nullptr);

	// Play credits screen VO
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::Credits);
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
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::SelectShip);
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
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::GameOver);
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
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlayMenuVO(EMenuSoundVO::HighScores);
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

void USpaceShooterMenuController::OpenOptionsScreen()
{
	OptionsScreen = Cast<UOptionsScreen>(OpenScreen(OptionsScreenClass));
	ensure(OptionsScreen != nullptr);
}

void USpaceShooterMenuController::CloseOptionsScreen()
{
	CloseScreen(OptionsScreen);
	OptionsScreen = nullptr;
}

void USpaceShooterMenuController::OpenStatsScreen()
{
	StatsScreen = Cast<UStatsScreen>(OpenScreen(StatsScreenClass));
	ensure(StatsScreen != nullptr);
}

void USpaceShooterMenuController::CloseStatsScreen()
{
	CloseScreen(StatsScreen);
	OptionsScreen = nullptr;
}

void USpaceShooterMenuController::OpenSoundOptionsScreen()
{
	SoundOptionsScreen = Cast<USoundOptionsScreen>(OpenScreen(SoundOptionsScreenClass));
	ensure(SoundOptionsScreen != nullptr);
}

void USpaceShooterMenuController::CloseSoundOptionsScreen()
{
	CloseScreen(SoundOptionsScreen);
	SoundOptionsScreen = nullptr;
}

void USpaceShooterMenuController::OpenDataScreen()
{
	DataScreen = Cast<UDataScreen>(OpenScreen(DataScreenClass));
	ensure(DataScreen != nullptr);
}

void USpaceShooterMenuController::CloseDataScreen()
{
	CloseScreen(DataScreen);
	DataScreen = nullptr;
}

void USpaceShooterMenuController::PlayButtonClickSound()
{
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::ButtonClick);
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
