// Copyright 2024 Richard Skala

#include "UI/MainMenuWidget.h"

#include "Components/TextBlock.h"

void UMainMenuWidget::OnTick(float DeltaTime)
{
	// Note: This is set to update from the MainMenu level blueprint graph
	UpdateMainMenuTitleTextColor(DeltaTime);
}

void UMainMenuWidget::UpdateMainMenuTitleTextColor(float DeltaTime)
{
	// Update color hue shift
	ColorCyclingTimer += DeltaTime / SecondsPerCyclingMode;
	FLinearColor LerpedColor(FLinearColor::White);

	switch (ColorCyclingMode)
	{
		case EColorCyclingMode::RedToGreen:
			LerpedColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, ColorCyclingTimer);
			break;

		case EColorCyclingMode::GreenToBlue:
			LerpedColor = FMath::Lerp(FLinearColor::Green, FLinearColor::Blue, ColorCyclingTimer);
			break;

		case EColorCyclingMode::BlueToRed:
			LerpedColor = FMath::Lerp(FLinearColor::Blue, FLinearColor::Red, ColorCyclingTimer);
			break;

		default:
			break;
	}

	if (MainMenuTitleText != nullptr)
	{
		MainMenuTitleText->SetColorAndOpacity(FSlateColor(LerpedColor));
	}

	if (ColorCyclingTimer >= 1.0f)
	{
		SwitchToNextColorCyclingMode();
		ColorCyclingTimer = 0.0f;
	}
}

void UMainMenuWidget::SwitchToNextColorCyclingMode()
{
	uint8 NextColorCyclingMode = ((uint8)ColorCyclingMode + 1) % (uint8)EColorCyclingMode::NumColorCyclingModes;
	ColorCyclingMode = (EColorCyclingMode)NextColorCyclingMode;

	//uint8 NextColorCyclingMode = (uint8)ColorCyclingMode + 1;
	//if (NextColorCyclingMode >= (uint8)EColorCyclingMode::NumColorCyclingModes)
	//{
	//	NextColorCyclingMode = (uint8)EColorCyclingMode::RedToGreen;
	//}
}
