// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuScreenBorder.generated.h"

// Widget defining a border around a menu. Supports color shifting.
UCLASS()
class SPACESHOOTER02_API UMenuScreenBorder : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateBorderImageColors(FLinearColor LinearColor);

protected:
	void SetColorForImage(class UImage* Image, FLinearColor LinearColor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_T;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UImage> Image_Screen_Border_B;
};
