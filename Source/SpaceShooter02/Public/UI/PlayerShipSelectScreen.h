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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBackButtonHovered();

	UFUNCTION()
	void OnShipSelectionLaunchButton1Hovered();

	UFUNCTION()
	void OnShipSelectionLaunchButton2Hovered();

	UFUNCTION()
	void OnShipSelectionLaunchButton3Hovered();

	UFUNCTION()
	void OnShipSelectionLaunchButton4Hovered();

	UFUNCTION()
	void OnShipSelectionLaunchButton5Hovered();

	void SetKeyboardFocusForWidgetLaunchButton(class UShipSelectionWidget* const ShipSelectionWidget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> SelectYourShipTextBlock;

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
