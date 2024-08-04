// Copyright 2024 Richard Skala

#include "UI/ShipSelectionWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PaperSprite.h"
#include "PaperSpriteBlueprintLibrary.h"

#include "UI/SpaceShooterMenuController.h"

void UShipSelectionWidget::InitShipSelectionWidget(FString ShipNameString, FString ShipAbilityString, FLinearColor ShipAbilityColor, UPaperSprite* InShipSprite)
{
	ShipSprite = InShipSprite;

	if (ShipNameTextBlock != nullptr)
	{
		ShipNameTextBlock->SetText(FText::FromString(ShipNameString));
		ShipNameTextBlock->SetColorAndOpacity(ShipAbilityColor);
	}

	if (ShipAbilityTextBlock != nullptr)
	{
		ShipAbilityTextBlock->SetText(FText::FromString(ShipAbilityString));
		ShipAbilityTextBlock->SetColorAndOpacity(ShipAbilityColor);
	}

	if (ShipImage != nullptr && ShipSprite != nullptr)
	{
		// The class UPaperSpriteBlueprintLibrary does NOT have the API macro in front so this causes a linker error
		// That is, it _should_ look like this:
		// class PAPER2D_API UPaperSpriteBlueprintLibrary : public UBlueprintFunctionLibrary
		//
		// So I've just copied (and slightly modified) that code here.
		// TODO: Create a SpaceShooterFunctionLibrary
		//FSlateBrush ShipSpriteBrush = UPaperSpriteBlueprintLibrary::MakeBrushFromSprite(ShipSprite, 64, 64);
		
		const FSlateAtlasData SpriteAtlasData = ShipSprite->GetSlateAtlasData();
		const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

		FSlateBrush ShipSpriteBrush;
		ShipSpriteBrush.SetResourceObject(ShipSprite);
		ShipSpriteBrush.ImageSize = FVector2D(SpriteSize.X, SpriteSize.Y);

		// Set the Sprite brush
		ShipImage->SetBrush(ShipSpriteBrush);

		//UTexture2D* ShipTexture = ShipSprite->GetSourceTexture(); // Note: This gives the WHOLE texture sprite sheet!
		//ShipImage->SetBrushFromTexture(ShipTexture);
	}
}

void UShipSelectionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LaunchButton != nullptr)
	{
		LaunchButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnLaunchButtonClicked);
	}
}

void UShipSelectionWidget::OnLaunchButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UShipSelectionWidget::OnLaunchButtonClicked - %s"), *GetName());
	ShipSelected();
}

void UShipSelectionWidget::ShipSelected()
{
	UE_LOG(LogTemp, Warning, TEXT("UShipSelectionWidget::OnLaunchButtonClicked - %s"), *GetName());
	USpaceShooterMenuController::OnPlayerShipSelected.Broadcast(ShipSprite);
}
