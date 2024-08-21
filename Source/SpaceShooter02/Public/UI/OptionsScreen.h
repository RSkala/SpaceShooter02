// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "OptionsScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UOptionsScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnColorShift(FLinearColor LinearColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const override { return CreditsButton; }

private:
	UFUNCTION() void OnCreditsButtonClicked();
	UFUNCTION() void OnBackButtonClicked();
	UFUNCTION() void OnClearScoresButtonClicked();
	UFUNCTION() void OnStatsButtonClicked();
	UFUNCTION() void OnClearStatsButtonClicked();

	UFUNCTION() void OnCreditsButtonHovered();
	UFUNCTION() void OnClearScoresButtonHovered();
	UFUNCTION() void OnBackButtonHovered();
	UFUNCTION() void OnStatsButtonHovered();
	UFUNCTION() void OnClearStatsButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> OptionsTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> CreditsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> StatsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> ClearScoresButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> ClearStatsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;
};
