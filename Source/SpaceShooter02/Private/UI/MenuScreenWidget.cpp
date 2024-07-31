// Copyright 2024 Richard Skala

#include "UI/MenuScreenWidget.h"

#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include "ColorShiftLocalPlayerSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UMenuScreenWidget, LogMenus)

void UMenuScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Subscribe to the color shift delegate
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UColorShiftLocalPlayerSubsystem* ColorShiftSubsystem = ULocalPlayer::GetSubsystem<UColorShiftLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			ColorShiftSubsystem->OnColorShift.AddUniqueDynamic(this, &ThisClass::OnColorShift);
		}
	}
}

void UMenuScreenWidget::OnColorShift(FLinearColor LinearColor)
{
	SetColorForImage(Image_Screen_Border_L, LinearColor);
	SetColorForImage(Image_Screen_Border_R, LinearColor);
	SetColorForImage(Image_Screen_Border_T, LinearColor);
	SetColorForImage(Image_Screen_Border_B, LinearColor);
}

void UMenuScreenWidget::SetColorForImage(UImage* Image, FLinearColor LinearColor)
{
	if (Image != nullptr)
	{
		Image->SetColorAndOpacity(LinearColor);
	}
}
