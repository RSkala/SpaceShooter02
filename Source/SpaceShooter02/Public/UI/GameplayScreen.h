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
	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION()
	void OnPlayerScoreUpdated(int32 PlayerScore);

	UFUNCTION()
	void OnPlayerScoreMultiplierUpdated(int32 PlayerScoreMultiplier);

	UFUNCTION()
	void OnPlayerHighScoreUpdated(int32 PlayerHighScore);

	UFUNCTION()
	void OnPowerupTimeUpdated(float Percent);

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
