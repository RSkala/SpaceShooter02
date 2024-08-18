// Copyright 2024 Richard Skala

#include "UI/OptionsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "UI/SpaceShooterMenuController.h"

void UOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, CreditsButton);
	}
}

void UOptionsScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	// HACK workaround to force keyboard focus if all buttons lose focus.
	// This will occur if the user clicks the mouse outside of any button.
	if (CreditsButton != nullptr && BackButton != nullptr)
	{
		if (!CreditsButton->HasKeyboardFocus()
			&& !BackButton->HasKeyboardFocus())
		{
			UE_LOG(LogTemp, Warning, TEXT("UMainMenuScreen::NativeTick - No buttons have keyboard focus. Forcing to CreditsButton"));
			CreditsButton->SetKeyboardFocus();
		}
	}
}

void UOptionsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (OptionsTextBlock != nullptr)
	{
		OptionsTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(CreditsButton, LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
}

void UOptionsScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenCreditsClicked.Broadcast();
}

void UOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenBackClicked.Broadcast();
}

void UOptionsScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
