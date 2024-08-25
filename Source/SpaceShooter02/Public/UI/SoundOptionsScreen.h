// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "SoundOptionsScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API USoundOptionsScreen : public UMenuScreenWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnColorShift(FLinearColor LinearColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

private:
	UFUNCTION() void OnMusicSelectButtonClicked();
	UFUNCTION() void OnSoundEffectsOnOffButtonClicked();
	UFUNCTION() void OnVOOnOffButton();
	UFUNCTION() void OnBackButtonClicked();

	UFUNCTION() void OnMusicSelectButtonHovered();
	UFUNCTION() void OnSoundEffectsOnOffButtonHovered();
	UFUNCTION() void OnVOOnOffButtonHovered();
	UFUNCTION() void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> SoundsTitleTextBlock;

	// --- Buttons ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> MusicSelectButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> SoundEffectsOnOffButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> VOOnOffButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	// -- Button Texts ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> MusicSelectButtonTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> SoundEffectsOnOffButtonTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> VOOnOffButtonTextBlock;
};
