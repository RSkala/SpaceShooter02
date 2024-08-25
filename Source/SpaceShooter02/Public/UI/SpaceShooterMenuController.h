// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpaceShooterMenuController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuPlayClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShipSelectedDelegateSignature, int32, ShipSpriteIndex);

// Game Over Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverSelectShipClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverPlayAgainClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverMainMenuClickedDelegateSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuCreditsClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShipSelectBackButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuHighScoreClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHighScoreBackButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuOptionsButtonClickedDelegateSignature);

// Options Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenHowToPlayButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenCreditsButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenStatsButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenSoundsButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenDataButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenBackButtonClickedDelegateSignature);

// How to Play Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHowToPlayScreenBackButtonClickedDelegateSignature);

// Sound Options Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundScreenMusicSelectClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundScreenSoundEffectClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundScreenVOClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundBackClickedDelegateSignature);

// Data Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDataScreenClearScoresClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDataScreenClearStatsClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDataScreenBackClickedDelegateSignature);

// Credits Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreditsScreenBackButtonClickedDelegateSignature);

// Stats Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatsScreenBackButtonClickedDelegateSignature, float, TimeSpentLookingAtStats);

UENUM(BlueprintType)
enum class EMenuState : uint8
{
	None,
	MainMenu,
	ShipSelect,
	Gameplay,
	GameOver,
	NumMenuStates UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EMenuState, EMenuState::NumMenuStates);

UCLASS(Blueprintable)
class SPACESHOOTER02_API USpaceShooterMenuController : public UObject
{
	GENERATED_BODY()

public:
	USpaceShooterMenuController();
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	void StartMainMenu();
	void ForceClosePauseScreen();

private:
	void OnPlayerShipSelectStart();

	UFUNCTION() void OnGameplayStart();
	UFUNCTION() void OnGameplayEnd(
		int32 FinalScore,
		int32 SelectedShipSpriteIndex,
		int32 NumEnemiesDefeated,
		int32 NumScoreMultipliersCollected,
		int32 NumEnemiesDefeatedWithBoost,
		int32 NumProjectilesFired,
		int32 CurrentScoreMultiplier,
		float GameplaySessionLength);
	UFUNCTION() void MainMenuPlayClicked();
	UFUNCTION() void PlayerShipSelected(int32 ShipSpriteIndex);

	UFUNCTION() void GameOverSelectShipClicked();
	UFUNCTION() void GameOverPlayAgainClicked();
	UFUNCTION() void GameOverMainMenuClicked();

	UFUNCTION() void MainMenuCreditsClicked();
	UFUNCTION() void ShipSelectBackClicked();
	UFUNCTION() void MainMenuHighScoreClicked();
	UFUNCTION() void HighScoreBackClicked();
	UFUNCTION() void PauseScreenResumeClicked();
	UFUNCTION() void MainMenuOptionsButtonClicked();

	// Options Screen actions
	UFUNCTION() void OptionsScreenHowToPlayClicked();
	UFUNCTION() void OptionsScreenCreditsClicked();
	UFUNCTION() void OptionsScreenStatsClicked();
	UFUNCTION() void OptionsScreenSoundsClicked();
	UFUNCTION() void OptionsScreenDataClicked();
	UFUNCTION() void OptionsScreenBackClicked();

	// How to Play Screen actions
	UFUNCTION() void HowToPlayBackClicked();

	// Sound Options Screen actions
	UFUNCTION() void SoundScreenMusicSelectClicked();
	UFUNCTION() void SoundScreenSoundEffectClicked();
	UFUNCTION() void SoundScreenVOClicked();
	UFUNCTION() void SoundScreenBackClicked();

	// Data Screen actions
	UFUNCTION() void DataScreenClearScoresClicked();
	UFUNCTION() void DataScreenClearStatsClicked();
	UFUNCTION() void DataScreenBackClicked();

	// Credits Screen actions
	UFUNCTION() void CreditsScreenBackClicked();

	// Stats Screen actions
	UFUNCTION() void StatsScreenBackClicked(float TimeSpentLookingAtStats);

	UUserWidget* OpenScreen(TSubclassOf<class UUserWidget> ScreenClass);
	void CloseScreen(UUserWidget* const ScreenToClose);

	// Main Menu / Title
	void OpenMainMenuScreen();
	void CloseMainMenuScreen();

	// How To Play
	void OpenHowToPlayScreen();
	void CloseHowToPlayScreen();

	// Credits
	void OpenCreditsScreen();
	void CloseCreditsScreen();

	// Ship Select
	void OpenPlayerShipSelectScreen();
	void ClosePlayerShipSelectScreen();

	// Game Over
	void OpenGameOverScreen(int32 FinalScore);
	void CloseGameOverScreen();

	// High Score
	void OpenHighScoreScreen();
	void CloseHighScoreScreen();

	// Pause
	void OpenPauseScreen();
	void ClosePauseScreen();

	// Options
	void OpenOptionsScreen();
	void CloseOptionsScreen();

	// Stats
	void OpenStatsScreen();
	void CloseStatsScreen();

	// Sound Options
	void OpenSoundOptionsScreen();
	void CloseSoundOptionsScreen();

	// Data
	void OpenDataScreen();
	void CloseDataScreen();

	void PlayButtonClickSound();

	UFUNCTION() void OnRequestPauseGame();
	UFUNCTION() void OnRequestUnpauseGame();

public:
	static FMainMenuPlayClickedDelegateSignature OnMainMenuPlayClicked;
	static FShipSelectedDelegateSignature OnPlayerShipSelected;

	// Game Over screen actions
	static FGameOverSelectShipClickedDelegateSignature OnGameOverSelectShipClicked;
	static FGameOverPlayAgainClickedDelegateSignature OnGameOverPlayAgainClicked;
	static FGameOverMainMenuClickedDelegateSignature OnGameOverMainMenuClicked;

	static FMainMenuCreditsClickedDelegateSignature OnMainMenuCreditsClicked;
	static FShipSelectBackButtonClickedDelegateSignature OnShipSelectBackClicked;
	static FMainMenuHighScoreClickedDelegateSignature OnMainMenuHighScoreClicked;
	static FHighScoreBackButtonClickedDelegateSignature OnHighScoreBackClicked;
	static FMainMenuOptionsButtonClickedDelegateSignature OnMainMenuOptionsClicked;

	// Options Screen Actions
	static FOptionsScreenHowToPlayButtonClickedDelegateSignature OnOptionsScreenHowToPlayClicked;
	static FOptionsScreenCreditsButtonClickedDelegateSignature OnOptionsScreenCreditsClicked;
	static FOptionsScreenStatsButtonClickedDelegateSignature OnOptionsScreenStatsClicked;
	static FOptionsScreenSoundsButtonClickedDelegateSignature OnOptionsScreenSoundsClicked;
	static FOptionsScreenDataButtonClickedDelegateSignature OnOptionsScreenDataClicked;
	static FOptionsScreenBackButtonClickedDelegateSignature OnOptionsScreenBackClicked;

	// How to Play Screen Actions
	static FHowToPlayScreenBackButtonClickedDelegateSignature OnHowToPlayScreenBackClicked;

	// Sound Options Screen Actions
	static FSoundScreenMusicSelectClickedDelegateSignature OnSoundScreenMusicSelectClicked;
	static FSoundScreenSoundEffectClickedDelegateSignature OnSoundScreenSoundEffectClicked;
	static FSoundScreenVOClickedDelegateSignature OnSoundScreenVOClicked;
	static FSoundBackClickedDelegateSignature OnSoundScreenBackClicked;

	// Data Screen Actions
	static FDataScreenClearScoresClickedDelegateSignature OnDataScreenClearScoresClicked;
	static FDataScreenClearStatsClickedDelegateSignature OnDataScreenClearStatsClicked;
	static FDataScreenBackClickedDelegateSignature OnDataScreenBackClicked;

	// Credits Screen Actions
	static FCreditsScreenBackButtonClickedDelegateSignature OnCreditsScreenBackClicked;

	// Stats Screen Actions
	static FStatsScreenBackButtonClickedDelegateSignature OnStatsScreenBackClicked;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMenuState CurrentMenuState = EMenuState::None;

	// --- Menu Screen Classes ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> MainMenuScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> PlayerShipSelectScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> GameOverScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> HowToPlayScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> CreditsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> HighScoreScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> PauseScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> OptionsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> StatsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> SoundOptionsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> DataScreenClass;

	// --- Menu Screen Instances ---

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMainMenuScreen> MainMenuScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPlayerShipSelectScreen> PlayerShipSelectScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGameOverScreen> GameOverScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHowToPlayScreen> HowToPlayScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGameCreditsScreen> CreditsScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHighScoreScreen> HighScoreScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPauseScreen> PauseScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOptionsScreen> OptionsScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatsScreen> StatsScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundOptionsScreen> SoundOptionsScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UDataScreen> DataScreen;
};
