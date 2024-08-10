// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuScreenWidget.h"
#include "GameOverScreen.generated.h"

UCLASS()
class SPACESHOOTER02_API UGameOverScreen : public UMenuScreenWidget
{
	GENERATED_BODY()

public:
	void InitGameOverScreen(int32 FinalScore);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual void OnColorShift(FLinearColor LinearColor) override;

	UFUNCTION()
	void OnPlayAgainButtonClicked();

	UFUNCTION()
	void OnSelectNewShipButtonClicked();

	UFUNCTION()
	void OnQuitGameButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> GameOverText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> FinalScoreText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> PlayAgainButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> SelectNewShipButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> QuitGameButton;
};
