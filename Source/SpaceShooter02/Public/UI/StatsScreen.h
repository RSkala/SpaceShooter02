// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "StatsScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UStatsScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void OnColorShift(FLinearColor LinearColor);
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

private:
	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> StatsTitleTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UVerticalBox> StatListVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TSubclassOf<class UStatDisplayWidget> StatDisplayWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class UStatDisplayWidget>> StatDisplayWidgets;

	// --- General Stats ----

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumGamesPlayedStatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumEnemiesDefeatedStatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumScoreMultipliersCollectedStatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumEnemiesDefeatedWithBoostStatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumProjectilesFiredStatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> HighestScoreMultiplierStatDisplay;

	// --- Ship Selection Stats ----

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumTimesSelectedShip1StatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumTimesSelectedShip2StatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumTimesSelectedShip3StatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumTimesSelectedShip4StatDisplay;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> NumTimesSelectedShip5StatDisplay;

	// --- Other ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStatDisplayWidget> TimeSpentLookingAtStatsStatDisplay;


	// -------------------------------------------------------------------------

	float SavedTimeSpentLookingAtStats = 0; // Time looking at stats from save data
	float TimeSpentLookingAtStats = 0; // Time looking at stats this session (of opening the stats screen)
};
