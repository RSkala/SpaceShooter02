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
	virtual void NativeConstruct() override;
	virtual void OnColorShift(FLinearColor LinearColor);

private:
	UFUNCTION()
	void OnBackButtonClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> CreditsTextBlock; // TODO: Change to RichTextBlock

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UButton> BackButton;

	// --- Lines for Credits Text ---
	static FString CreditsLine1;
	static FString CreditsLine2;
	static FString CreditsLine3;
	static FString CreditsLine4;
	static FString CreditsLine5;
	static FString CreditsLine6;
};
