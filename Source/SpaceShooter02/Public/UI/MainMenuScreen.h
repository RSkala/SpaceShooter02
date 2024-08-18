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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION() void OnPlayButtonClicked();
	UFUNCTION() void OnOptionsButtonClicked();
	UFUNCTION() void OnExitButtonClicked();
	UFUNCTION() void OnCreditsButtonClicked();
	UFUNCTION() void OnHighScoresButtonClicked();

	UFUNCTION() void OnPlayButtonHovered();
	UFUNCTION() void OnOptionsButtonHovered();
	UFUNCTION() void OnExitButtonHovered();
	UFUNCTION() void OnCreditsButtonHovered();
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
	TObjectPtr<class UButton> CreditsButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> HighScoresButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> VersionText;

	// May use this in the future if going from Main Menu -> Ship Select is annoying
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	//TObjectPtr<class UButton> ShipSelectButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USoundBase> VOTestSound;
};
