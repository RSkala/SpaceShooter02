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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenCreditsButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenStatsButtonClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenClearScoresClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenClearStatsClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenBackButtonClickedDelegateSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenMusicSelectClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsScreenSoundEffectClickedDelegateSignature);

// Credits Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreditsScreenBackButtonClickedDelegateSignature);

// Stats Screen actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatsScreenBackButtonClickedDelegateSignature, float, TimeSpentLookingAtStats);

// Enum for tracking whether or not a sound VO was played
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESoundVOPlayed : uint8
{
	None = (0x0) UMETA(Hidden),
	CreditsVOPlayed = 1 << 0,
	GameOverVOPlayed = 1 << 2,
	GoodLuckVOPlayed = 1 << 3,
	SelectShipVOPlayed = 1 << 4,
	TitleVOPlayed = 1 << 5,
	WelcomeBackVOPlayed = 1 << 6,
	HighScoresVOPlayed = 1 << 7
};
ENUM_CLASS_FLAGS(ESoundVOPlayed);

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
		int32 CurrentScoreMultiplier);
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

	UFUNCTION() void OptionsScreenCreditsClicked();
	UFUNCTION() void OptionsScreenStatsClicked();
	UFUNCTION() void OptionsScreenClearScoresClicked();
	UFUNCTION() void OptionsScreenClearStatsClicked();
	UFUNCTION() void OptionsScreenBackClicked();
	UFUNCTION() void OptionsScreenMusicSelectClicked();
	UFUNCTION() void OptionsScreenSoundEffectClicked();

	UFUNCTION() void CreditsScreenBackClicked();

	UFUNCTION() void StatsScreenBackClicked(float TimeSpentLookingAtStats);

	UUserWidget* OpenScreen(TSubclassOf<class UUserWidget> ScreenClass);
	void CloseScreen(UUserWidget* const ScreenToClose);

	void OpenMainMenuScreen();
	void CloseMainMenuScreen();

	void OpenCreditsScreen();
	void CloseCreditsScreen();

	void OpenPlayerShipSelectScreen();
	void ClosePlayerShipSelectScreen();

	void OpenGameOverScreen(int32 FinalScore);
	void CloseGameOverScreen();

	void OpenHighScoreScreen();
	void CloseHighScoreScreen();

	void OpenPauseScreen();
	void ClosePauseScreen();

	void OpenOptionsScreen();
	void CloseOptionsScreen();

	void OpenStatsScreen();
	void CloseStatsScreen();

	void SelectAndPlayRandomVO(TArray<TSoftObjectPtr<USoundBase>> SoundVOArray);

	bool HasSoundVOBeenPlayed(ESoundVOPlayed SoundVOPlayed) const;
	void SetSoundVOPlayed(ESoundVOPlayed SoundVOPlayed);

	void PlayButtonClickSound();

	UFUNCTION()
	void OnRequestPauseGame();

	UFUNCTION()
	void OnRequestUnpauseGame();

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
	static FOptionsScreenCreditsButtonClickedDelegateSignature OnOptionsScreenCreditsClicked;
	static FOptionsScreenStatsButtonClickedDelegateSignature OnOptionsScreenStatsClicked;
	static FOptionsScreenClearScoresClickedDelegateSignature OnOptionsScreenClearScoresClicked;
	static FOptionsScreenClearStatsClickedDelegateSignature OnOptionsScreenClearStatsClicked;
	static FOptionsScreenBackButtonClickedDelegateSignature OnOptionsScreenBackClicked;
	static FOptionsScreenMusicSelectClickedDelegateSignature OnOptionsMusicSelectClicked;
	static FOptionsScreenSoundEffectClickedDelegateSignature OnOptionsSoundEffectClicked;

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
	TSubclassOf<class UUserWidget> CreditsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> HighScoreScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> PauseScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> OptionsScreenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> StatsScreenClass;

	// --- Menu Screen Instances ---

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMainMenuScreen> MainMenuScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPlayerShipSelectScreen> PlayerShipSelectScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGameOverScreen> GameOverScreen;

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

	// --- Menu VO ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> CreditsVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> GameOverVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> GoodLuckVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> SelectShipVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> TitleVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> WelcomeBackVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> HighScoreVOSounds;

	// --- Flags for limiting VO sound playing ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BitMask, BitmaskEnum = "/Script/SpaceShooter02.ESoundVOPlayed", AllowPrivateAccess = true))
	uint8 SoundVOPlayedFlags;

	// Currently playing VO sound
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAudioComponent> CurrentVOSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> UIButtonClickSound;
};
