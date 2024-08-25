// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "HowToPlayScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UHowToPlayScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnColorShift(FLinearColor LinearColor);
	virtual class UButton* GetKeyboardFocusLostButton() const override { return BackButton; }

private:
	UFUNCTION() void OnBackButtonClicked();
	UFUNCTION() void OnBackButtonHovered();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> HowToPlayTitleTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> HowToPlayTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	// --- Lines for How to Play Text ---

	// Note: These are UPROPERTY instance strings and NOT statics, as static variables are NOT refreshed with LiveCoding

	UPROPERTY() FString HowToPlayLine1;
	UPROPERTY() FString HowToPlayLine2;
	UPROPERTY() FString HowToPlayLine3;
	UPROPERTY() FString HowToPlayLine4;
	UPROPERTY() FString HowToPlayLine5;
	UPROPERTY() FString HowToPlayLine6;
	UPROPERTY() FString HowToPlayLine7;
};
