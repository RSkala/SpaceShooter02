// Copyright 2024 Richard Skala

#include "ColorShiftLocalPlayerSubsystem.h"

// DECLARE_CYCLE_STAT(TEXT("Tick ColorShift"), STAT_TickColorShift, STATGROUP_SpaceShooter);

DEFINE_LOG_CATEGORY_STATIC(LogColorShift, Log, All);

bool UColorShiftLocalPlayerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UColorShiftLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UColorShiftLocalPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UColorShiftLocalPlayerSubsystem::Tick(float DeltaTime)
{
	// Ensure we don't tick multiple times per frame
	if (LastFrameNumberTicked == GFrameCounter)
	{
		return;
	}

	// Update the color shift
	UpdateColorShift(DeltaTime);

	// Set the last frame ticked
	LastFrameNumberTicked = GFrameCounter;
}

ETickableTickType UColorShiftLocalPlayerSubsystem::GetTickableTickType() const
{
	// Do not allow ticking in the CDO. The CDO will tick if this line is not here.
	return HasAnyFlags(RF_ClassDefaultObject) ? ETickableTickType::Never : ETickableTickType::Always;
}

void UColorShiftLocalPlayerSubsystem::UpdateColorShift(float DeltaTime)
{
	// Update color hue shift
	ColorCyclingTimer += DeltaTime / SecondsPerCyclingMode;
	FLinearColor LerpedColor(FLinearColor::White);

	switch (ColorCyclingMode)
	{
		case EColorCyclingMode::RedToGreen: LerpedColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, ColorCyclingTimer); break;
		case EColorCyclingMode::GreenToBlue: LerpedColor = FMath::Lerp(FLinearColor::Green, FLinearColor::Blue, ColorCyclingTimer); break;
		case EColorCyclingMode::BlueToRed: LerpedColor = FMath::Lerp(FLinearColor::Blue, FLinearColor::Red, ColorCyclingTimer); break;
		default: break;
	}

	// Update color on all subscribers
	OnColorShift.Broadcast(LerpedColor);

	// If the color cycling timer has elapsed, change color cycling modes
	if (ColorCyclingTimer >= 1.0f)
	{
		SwitchToNextColorCyclingMode();
		ColorCyclingTimer = 0.0f;
	}

	//UE_LOG(LogColorShift, Log, TEXT("ColorCyclingTimer: %f, ColorCyclingMode: %s"), ColorCyclingTimer, *ColorShiftEnumToString(ColorCyclingMode));
}

void UColorShiftLocalPlayerSubsystem::SwitchToNextColorCyclingMode()
{
	uint8 NextColorCyclingMode = ((uint8)ColorCyclingMode + 1) % (uint8)EColorCyclingMode::NumColorCyclingModes;
	ColorCyclingMode = (EColorCyclingMode)NextColorCyclingMode;

	//UE_LOG(LogColorShift, Log, TEXT("Switching to ColorShiftMode: %s, ColorCyclingTimer: %f"), *ColorShiftEnumToString(ColorCyclingMode), ColorCyclingTimer);

	//uint8 NextColorCyclingMode = (uint8)ColorCyclingMode + 1;
	//if (NextColorCyclingMode >= (uint8)EColorCyclingMode3::NumColorCyclingModes)
	//{
	//	NextColorCyclingMode = (uint8)EColorCyclingMode3::RedToGreen;
	//}
}

/*static*/ FString UColorShiftLocalPlayerSubsystem::ColorShiftEnumToString(EColorCyclingMode ColorCyclingMode)
{
	FString ColorCyclingModeString;
	switch (ColorCyclingMode)
	{
		case EColorCyclingMode::RedToGreen: ColorCyclingModeString = TEXT("RedToGreen"); break;
		case EColorCyclingMode::GreenToBlue: ColorCyclingModeString = TEXT("GreenToBlue"); break;
		case EColorCyclingMode::BlueToRed: ColorCyclingModeString = TEXT("BlueToRed"); break;
		default: ColorCyclingModeString = TEXT("Invalid"); break;
	}
	return ColorCyclingModeString;
}
