// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreDisplayWidget.generated.h"

UCLASS()
class SPACESHOOTER02_API UScoreDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScoreInfoFromScoreData(const struct FHighScoreData& HighScoreData, int32 Rank);
	void SetColorShift(FLinearColor ShiftColor);

protected:
	virtual void NativeDestruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> RankTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> ScoreTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> DateTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UTextBlock> SeparatorTextBlock;

	// Displays a small picture of the ship used to earn this score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<class UImage> ShipImage;
};
