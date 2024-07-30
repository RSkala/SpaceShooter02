// Copyright 2024 Richard Skala

#include "UI/MainMenuWidget.h"

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
	uint8 NextColorCyclingMode = ((uint8)ColorCyclingMode + 1) % (uint8)EMainMenuColorCyclingMode::NumColorCyclingModes;
	ColorCyclingMode = (EMainMenuColorCyclingMode)NextColorCyclingMode;

	//uint8 NextColorCyclingMode = (uint8)ColorCyclingMode + 1;
	//if (NextColorCyclingMode >= (uint8)EColorCyclingMode::NumColorCyclingModes)
	//{
	//	NextColorCyclingMode = (uint8)EColorCyclingMode::RedToGreen;
	//}
}
