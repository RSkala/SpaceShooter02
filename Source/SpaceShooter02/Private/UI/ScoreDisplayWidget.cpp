// Copyright 2024 Richard Skala

#include "UI/ScoreDisplayWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "PaperSprite.h"

#include "SpaceShooterGameInstance.h"
#include "SpaceShooterSaveGame.h"

#define LOCTEXT_NAMESPACE "ScoreDisplayWidget"

void UScoreDisplayWidget::SetScoreInfoFromScoreData(const FHighScoreData& HighScoreData, int32 Rank)
{
	// Display the string in the format "Rank. Score ...... Date", e.g.
	// 1. 34,245,856 ...... 2024.08.12
	if (ScoreInfoTextBlock != nullptr)
	{
		// Rank
		FString RankTextString = Rank < 10 ? FString::Printf(TEXT(" %d."), Rank) : FString::Printf(TEXT("%d."), Rank);
		FText RankText = FText::FromString(FString::Printf(TEXT("%s"), *RankTextString));

		// Score
		FText ScoreDisplayTextGrouped = UKismetTextLibrary::Conv_IntToText(HighScoreData.HighScore, false, true);

		// Date
		FText DateText = FText::FromString(HighScoreData.DateEarned); // This string was already formatted when saved

		// Format the text
		static const FText ScoreTextFormat = LOCTEXT("ScoreDisplayText", "{0}{1} ...... {2}");
		FText ScoreDisplayText = FText::Format(ScoreTextFormat, RankText, ScoreDisplayTextGrouped, DateText);

		// Set the formatted text
		ScoreInfoTextBlock->SetText(ScoreDisplayText);
	}

	// Set the ship image sprite
	if (ShipImage != nullptr)
	{
		if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			UPaperSprite* ShipSprite = GameInstance->GetShipSpriteForIndex(HighScoreData.ShipSpriteIndex);
			if (ShipSprite != nullptr)
			{
				const FSlateAtlasData SpriteAtlasData = ShipSprite->GetSlateAtlasData();
				const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

				FSlateBrush ShipSpriteBrush;
				ShipSpriteBrush.SetResourceObject(ShipSprite);
				ShipSpriteBrush.ImageSize = FVector2D(SpriteSize.X, SpriteSize.Y);
				FVector2D DesiredSize = ShipImage->GetDesiredSize();
				ShipImage->SetBrush(ShipSpriteBrush);

				// Note: This causes linker errors, so you can't use it:
				// FSlateBrush ShipSpriteBrush = UPaperSpriteBlueprintLibrary::MakeBrushFromSprite(ShipSprite, 64, 64);
			}
		}
	}
}

void UScoreDisplayWidget::SetColorShift(FLinearColor ShiftColor)
{
	if (ScoreInfoTextBlock != nullptr)
	{
		ScoreInfoTextBlock->SetColorAndOpacity(FSlateColor(ShiftColor));
	}
}

void UScoreDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

#undef LOCTEXT_NAMESPACE
