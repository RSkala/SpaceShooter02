// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "GameplayScreen.generated.h"

// Gameplay screen used as the player's HUD
UCLASS()
class SPACESHOOTER02_API UGameplayScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeSupportsCustomNavigation() const override { return false; }
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& NavigationEvent) override;

	virtual void OnColorShift(FLinearColor LinearColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const override { return nullptr; } // Gameplay Screen does not have a focusable widget

	UFUNCTION()
	void OnPlayerScoreUpdated(int32 PlayerScore);

	UFUNCTION()
	void OnPlayerScoreMultiplierUpdated(int32 PlayerScoreMultiplier);

	UFUNCTION()
	void OnPlayerHighScoreUpdated(int32 PlayerHighScore);

	UFUNCTION()
	void OnPowerupTimeUpdated(float Percent);

	UFUNCTION()
	void OnDashRechargedUpdated(float Percent);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> CurrentScoreText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> CurrentMultiplierText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> HighScoreText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UProgressBar> PowerupWeaponMeter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UProgressBar> DashMeter;
};
