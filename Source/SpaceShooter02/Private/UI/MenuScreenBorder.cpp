// Copyright 2024 Richard Skala

#include "UI/MenuScreenBorder.h"

#include "Components/Image.h"

void UMenuScreenBorder::UpdateBorderImageColors(FLinearColor LinearColor)
{
	SetColorForImage(Image_Screen_Border_L, LinearColor);
	SetColorForImage(Image_Screen_Border_R, LinearColor);
	SetColorForImage(Image_Screen_Border_T, LinearColor);
	SetColorForImage(Image_Screen_Border_B, LinearColor);
}

void UMenuScreenBorder::SetColorForImage(UImage* Image, FLinearColor LinearColor)
{
	if (Image != nullptr)
	{
		Image->SetColorAndOpacity(LinearColor);
	}
}
