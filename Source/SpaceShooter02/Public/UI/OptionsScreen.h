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
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

private:
	UFUNCTION() void OnHowToPlayButtonClicked();
	UFUNCTION() void OnCreditsButtonClicked();
	UFUNCTION() void OnStatsButtonClicked();
	UFUNCTION() void OnSoundsButtonClicked();
	UFUNCTION() void OnDataButtonClicked();
	UFUNCTION() void OnBackButtonClicked();

	UFUNCTION() void OnHowToPlayButtonHovered();
	UFUNCTION() void OnCreditsButtonHovered();
	UFUNCTION() void OnStatsButtonHovered();
	UFUNCTION() void OnSoundsButtonHovered();
	UFUNCTION() void OnDataButtonHovered();
	UFUNCTION() void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> OptionsTextBlock;

	// --- Buttons ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> HowToPlayButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> CreditsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> StatsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> SoundsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> DataButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;
};
