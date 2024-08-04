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

	UFUNCTION() void OnBackButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> SelectYourShipTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UPaperSprite>> ShipSprites;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UButton> BackButton;

private:
	// --- Ship Selection Widgets ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UShipSelectionWidget> ShipSelectionWidget1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UShipSelectionWidget> ShipSelectionWidget2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UShipSelectionWidget> ShipSelectionWidget3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UShipSelectionWidget> ShipSelectionWidget4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UShipSelectionWidget> ShipSelectionWidget5;
};
