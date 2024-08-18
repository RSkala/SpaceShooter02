// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "PauseScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UPauseScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnColorShift(FLinearColor ShiftColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const override { return ResumeButton; }

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnResumeButtonHovered();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnQuitButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> PausedTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> ResumeButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> QuitGameButton;
};
