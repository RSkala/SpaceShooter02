// Copyright 2024 Richard Skala

#include "UI/StatDisplayWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"

#include "SpaceShooterGameInstance.h"

void UStatDisplayWidget::SetStatNameText(FText StatNameText)
{
	if (StatNameTextBlock)
	{
		StatNameTextBlock->SetText(StatNameText);
	}
}

void UStatDisplayWidget::UpdateStatDataText(FText StatDataText)
{
	if (StatDataTextBlock != nullptr)
	{
		StatDataTextBlock->SetText(StatDataText);
	}
}

void UStatDisplayWidget::SetShipImageSpriteByIndex(int32 ShipSpriteIndex)
{
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		UPaperSprite* ShipSprite = GameInstance->GetShipSpriteForIndex(ShipSpriteIndex);
		if (ShipImage != nullptr && ShipSprite != nullptr)
		{
			const FSlateAtlasData SpriteAtlasData = ShipSprite->GetSlateAtlasData();
			const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

			FSlateBrush ShipSpriteBrush;
			ShipSpriteBrush.SetResourceObject(ShipSprite);
			ShipSpriteBrush.ImageSize = FVector2D(SpriteSize.X, SpriteSize.Y);
			FVector2D DesiredSize = ShipImage->GetDesiredSize();
			ShipImage->SetBrush(ShipSpriteBrush);

			ShipImage->SetVisibility(ESlateVisibility::Visible);

			// Note: This causes linker errors, so you can't use it:
			// FSlateBrush ShipSpriteBrush = UPaperSpriteBlueprintLibrary::MakeBrushFromSprite(ShipSprite, 64, 64);
		}
	}
}

void UStatDisplayWidget::SetColorShift(FLinearColor ShiftColor)
{
	if (StatNameTextBlock != nullptr)
	{
		StatNameTextBlock->SetColorAndOpacity(FSlateColor(ShiftColor));
	}

	if (StatDataTextBlock != nullptr)
	{
		StatDataTextBlock->SetColorAndOpacity(FSlateColor(ShiftColor));
	}
}

void UStatDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Clear the stat text
	if (StatNameTextBlock != nullptr)
	{
		StatNameTextBlock->SetText(FText());
	}

	if (StatDataTextBlock != nullptr)
	{
		StatDataTextBlock->SetText(FText());
	}

	// By default, hide the ship image
	if (ShipImage != nullptr)
	{
		ShipImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UStatDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
