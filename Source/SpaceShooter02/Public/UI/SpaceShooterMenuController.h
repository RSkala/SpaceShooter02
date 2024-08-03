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

protected:
	void OnPlayerShipSelectStart();

	UFUNCTION() void OnGameplayStart();
	UFUNCTION() void OnGameplayEnd();
	UFUNCTION() void MainMenuPlayClicked();
	UFUNCTION() void PlayerShipSelected(class UPaperSprite* SelectedShipSprite);
	UFUNCTION() void GameOverSelectShipClicked();
	UFUNCTION() void GameOverPlayAgainClicked();
	UFUNCTION() void MainMenuCreditsClicked();
	UFUNCTION() void CreditsMenuBackClicked();

	UUserWidget* OpenScreen(TSubclassOf<class UUserWidget> ScreenClass);
	void CloseScreen(UUserWidget* const ScreenToClose);

public:
	static FMainMenuPlayClickedDelegateSignature OnMainMenuPlayClicked;
	static FShipSelectedDelegateSignature OnPlayerShipSelected;
	static FGameOverSelectShipClickedDelegateSignature OnGameOverSelectShipClicked;
	static FGameOverPlayAgainClickedDelegateSignature OnGameOverPlayAgainClicked;
	static FMainMenuCreditsClickedDelegateSignature OnMainMenuCreditsClicked;
	static FCreditsMenuBackClickedDelegateSignature OnCreditsMenuBackClicked;

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
};
