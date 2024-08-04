// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "PlayerShipSelectScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UPlayerShipSelectScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION()
	void OnLaunchButtonClicked();

	UFUNCTION() void OnShipSelectButton1Clicked();
	UFUNCTION() void OnShipSelectButton2Clicked();
	UFUNCTION() void OnShipSelectButton3Clicked();
	UFUNCTION() void OnShipSelectButton4Clicked();
	UFUNCTION() void OnShipSelectButton5Clicked();

	UFUNCTION() void OnBackButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> LaunchButton; // TEMP: Will be removed once ship selection widget is implemented

	// --- Ship Select Button --- 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ShipSelectButton1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ShipSelectButton2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ShipSelectButton3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ShipSelectButton4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ShipSelectButton5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> SelectYourShipTextBlock;

	// --- Ship Sprites ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSprite> ShipSprite1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSprite> ShipSprite2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSprite> ShipSprite3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSprite> ShipSprite4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperSprite> ShipSprite5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UButton> BackButton;
};
