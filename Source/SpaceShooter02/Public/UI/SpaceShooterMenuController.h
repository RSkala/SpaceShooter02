// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpaceShooterMenuController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuPlayClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShipSelectedDelegateSignature, class UPaperSprite*, SelectedShipSprite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverSelectShipClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverPlayAgainClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuCreditsClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreditsMenuBackClickedDelegateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShipSelectBackButtonClickedDelegateSignature);

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
	WelcomeBackVOPlayed = 1 << 6
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

private:
	void OnPlayerShipSelectStart();

	UFUNCTION() void OnGameplayStart();
	UFUNCTION() void OnGameplayEnd();
	UFUNCTION() void MainMenuPlayClicked();
	UFUNCTION() void PlayerShipSelected(class UPaperSprite* SelectedShipSprite);
	UFUNCTION() void GameOverSelectShipClicked();
	UFUNCTION() void GameOverPlayAgainClicked();
	UFUNCTION() void MainMenuCreditsClicked();
	UFUNCTION() void CreditsMenuBackClicked();
	UFUNCTION() void ShipSelectBackClicked();

	UUserWidget* OpenScreen(TSubclassOf<class UUserWidget> ScreenClass);
	void CloseScreen(UUserWidget* const ScreenToClose);

	void OpenMainMenuScreen();
	void CloseMainMenuScreen();

	void OpenCreditsScreen();
	void CloseCreditsScreen();

	void OpenPlayerShipSelectScreen();
	void ClosePlayerShipSelectScreen();

	void OpenGameOverScreen();
	void CloseGameOverScreen();

	void SelectAndPlayRandomVO(TArray<TSoftObjectPtr<USoundBase>> SoundVOArray);

	bool HasSoundVOBeenPlayed(ESoundVOPlayed SoundVOPlayed) const;
	void SetSoundVOPlayed(ESoundVOPlayed SoundVOPlayed);

public:
	static FMainMenuPlayClickedDelegateSignature OnMainMenuPlayClicked;
	static FShipSelectedDelegateSignature OnPlayerShipSelected;
	static FGameOverSelectShipClickedDelegateSignature OnGameOverSelectShipClicked;
	static FGameOverPlayAgainClickedDelegateSignature OnGameOverPlayAgainClicked;
	static FMainMenuCreditsClickedDelegateSignature OnMainMenuCreditsClicked;
	static FCreditsMenuBackClickedDelegateSignature OnCreditsMenuBackClicked;
	static FShipSelectBackButtonClickedDelegateSignature OnShipSelectBackClicked;

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

	// --- Menu Screen Instances ---

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMainMenuScreen> MainMenuScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPlayerShipSelectScreen> PlayerShipSelectScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGameOverScreen> GameOverScreen;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGameCreditsScreen> CreditsScreen;

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

	// --- Flags for limiting VO sound playing ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BitMask, BitmaskEnum = "/Script/SpaceShooter02.ESoundVOPlayed", AllowPrivateAccess = true))
	uint8 SoundVOPlayedFlags;

	// Currently playing VO sound
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAudioComponent> CurrentVOSound;
};
