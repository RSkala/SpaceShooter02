// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatDisplayWidget.generated.h"

UCLASS()
class SPACESHOOTER02_API UStatDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatNameText(FText StatNameText);
	void UpdateStatDataText(FText StatDataText);
	void SetShipImageSpriteByIndex(int32 ShipSpriteIndex);
	void SetColorShift(FLinearColor ShiftColor);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> StatNameTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> StatDataTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UImage> ShipImage;
};
