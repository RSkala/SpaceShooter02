// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "MainMenuScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UMainMenuScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

public:
	UMainMenuScreen();

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnColorShift(FLinearColor LinearColor) override;
	virtual class UButton* GetKeyboardFocusLostButton() const { return PlayButton; }

	UFUNCTION() void OnPlayButtonClicked();
	UFUNCTION() void OnOptionsButtonClicked();
	UFUNCTION() void OnExitButtonClicked();
	UFUNCTION() void OnHighScoresButtonClicked();

	UFUNCTION() void OnPlayButtonHovered();
	UFUNCTION() void OnOptionsButtonHovered();
	UFUNCTION() void OnExitButtonHovered();
	UFUNCTION() void OnHighScoreButtonHovered();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> MainMenuTitleText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> PlayButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> OptionsButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> HighScoresButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> VersionText;
};
