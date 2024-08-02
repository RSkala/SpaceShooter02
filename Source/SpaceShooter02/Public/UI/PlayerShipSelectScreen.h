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

	UFUNCTION()
	void OnLaunchButtonClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> LaunchButton; // TEMP: Will be removed once ship selection widget is implemented
};
