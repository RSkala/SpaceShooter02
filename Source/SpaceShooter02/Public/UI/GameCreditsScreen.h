// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "GameCreditsScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UGameCreditsScreen : public UMenuScreenWidget
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
	TObjectPtr<class UTextBlock> CreditsTitleTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> CreditsTextBlock; // TODO: Change to RichTextBlock

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UImage> UnrealEngineLogoImage;

	// --- Lines for Credits Text ---
	static FString CreditsLine1;
	static FString CreditsLine2;
	static FString CreditsLine3;
	static FString CreditsLine4;
	static FString CreditsLine5;
	static FString CreditsLine6;
};
