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


	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply);
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& NavigationEvent) override;

	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION() void OnPlayButtonClicked();
	UFUNCTION() void OnExitButtonClicked();
	UFUNCTION() void OnCreditsButtonClicked();

	//UFUNCTION() void OnPlayButtonHovered();
	//UFUNCTION() void OnPlayButtonUnhovered();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> MainMenuTitleText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> PlayButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> CreditsButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> VersionText;

	// May use this in the future if going from Main Menu -> Ship Select is annoying
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	//TObjectPtr<class UButton> ShipSelectButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USoundBase> VOTestSound;
};
