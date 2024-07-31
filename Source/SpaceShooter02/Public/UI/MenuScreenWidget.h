// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuScreenWidget.generated.h"

UCLASS(Abstract)
class SPACESHOOTER02_API UMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_LOG_CATEGORY_CLASS(LogMenus, Log, All)

protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	virtual void OnColorShift(FLinearColor LinearColor);

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
