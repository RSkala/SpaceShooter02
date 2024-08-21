// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "StatsScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UStatsScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnColorShift(FLinearColor LinearColor);
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

private:
	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> StatsTitleTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;
};
