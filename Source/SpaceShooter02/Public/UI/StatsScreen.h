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

	// --- General Stats ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumGamesPlayedTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumEnemiesDefeatedTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumScoreMultipliersCollectedTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumEnemiesDefeatedWithBoostTextBlock;

	// --- Ship Selection Stats ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumTimesSelectedShip1TextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumTimesSelectedShip2TextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumTimesSelectedShip3TextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumTimesSelectedShip4TextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> NumTimesSelectedShip5TextBlock;

	// --- Other ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> TimeSpentLookingAtStatsTextBlock;

	float SavedTimeSpentLookingAtStats = 0; // Time looking at stats from save data
	float TimeSpentLookingAtStats = 0; // Time looking at stats this session (of opening the stats screen)
};
