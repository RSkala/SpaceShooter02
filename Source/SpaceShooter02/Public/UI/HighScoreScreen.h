// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "HighScoreScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UHighScoreScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual void OnColorShift(FLinearColor LinearColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> HighScoresTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UVerticalBox> HighScoreListVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> ScoreDisplayWidgetClass;

	UPROPERTY(meta = (AllowPrivateAccess = true))
	TArray<class UScoreDisplayWidget*> ScoreDisplayWidgets;
};
