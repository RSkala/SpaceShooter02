// Copyright 2024 Richard Skala

#include "UI/MainMenuWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMainMenuWidget::OnTick(float DeltaTime)
{
	// Note: This is set to update from the MainMenu level blueprint graph
	// Update: I forgot that UUserWidget has a NativeTick function, so this was disconnected.
	//UpdateMainMenuTitleTextColor(DeltaTime);
}

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateMainMenuTitleTextColor(InDeltaTime);
}

void UMainMenuWidget::UpdateMainMenuTitleTextColor(float DeltaTime)
{
	// Update color hue shift
	ColorCyclingTimer += DeltaTime / SecondsPerCyclingMode;
	FLinearColor LerpedColor(FLinearColor::White);

	switch (ColorCyclingMode)
	{
		case EMainMenuColorCyclingMode::RedToGreen:
			LerpedColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, ColorCyclingTimer);
			break;

		case EMainMenuColorCyclingMode::GreenToBlue:
			LerpedColor = FMath::Lerp(FLinearColor::Green, FLinearColor::Blue, ColorCyclingTimer);
			break;

		case EMainMenuColorCyclingMode::BlueToRed:
			LerpedColor = FMath::Lerp(FLinearColor::Blue, FLinearColor::Red, ColorCyclingTimer);
			break;

		default:
			break;
	}

	SetColorForTextBlock(MainMenuTitleText, LerpedColor);
	SetColorForImage(Image_Border_Left, LerpedColor);
	SetColorForImage(Image_Border_Right, LerpedColor);
	SetColorForImage(Image_Border_Top, LerpedColor);
	SetColorForImage(Image_Border_Bottom, LerpedColor);

	if (ColorCyclingTimer >= 1.0f)
	{
		SwitchToNextColorCyclingMode();
		ColorCyclingTimer = 0.0f;
	}
}

void UMainMenuWidget::SwitchToNextColorCyclingMode()
{
	uint8 NextColorCyclingMode = ((uint8)ColorCyclingMode + 1) % (uint8)EMainMenuColorCyclingMode::NumColorCyclingModes;
	ColorCyclingMode = (EMainMenuColorCyclingMode)NextColorCyclingMode;

	//uint8 NextColorCyclingMode = (uint8)ColorCyclingMode + 1;
	//if (NextColorCyclingMode >= (uint8)EColorCyclingMode::NumColorCyclingModes)
	//{
	//	NextColorCyclingMode = (uint8)EColorCyclingMode::RedToGreen;
	//}
}

void UMainMenuWidget::SetColorForTextBlock(UTextBlock* TextBlock, FLinearColor LinearColor)
{
	if (TextBlock != nullptr)
	{
		TextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}
}

void UMainMenuWidget::SetColorForImage(UImage* Image, FLinearColor LinearColor)
{
	if (Image != nullptr)
	{
		Image->SetColorAndOpacity(LinearColor);
	}
}
