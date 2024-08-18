// Copyright 2024 Richard Skala

#include "UI/PlayerShipSelectScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "PaperSprite.h"

#include "SpaceShooterGameState.h"
#include "UI/ShipSelectionWidget.h"
#include "UI/SpaceShooterMenuController.h"

void UPlayerShipSelectScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
	}

	// -------------------------------------------------
	if (ShipSelectionWidget1 != nullptr)
	{
		ShipSelectionWidget1->InitShipSelectionWidget(
			TEXT("SHIP 01"),
			TEXT("WHITE"),
			FLinearColor::White,
			0);

		if (UButton* LaunchButton = ShipSelectionWidget1->GetLaunchButton())
		{
			LaunchButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnShipSelectionLaunchButton1Hovered);
		}
	}

	if (ShipSelectionWidget2 != nullptr)
	{
		ShipSelectionWidget2->InitShipSelectionWidget(
			TEXT("SHIP 02"),
			TEXT("ORANGE"),
			FLinearColor(1.0f, 0.262251f, 0.0f),
			1);

		if (UButton* LaunchButton = ShipSelectionWidget2->GetLaunchButton())
		{
			LaunchButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnShipSelectionLaunchButton2Hovered);
		}
	}

	if (ShipSelectionWidget3 != nullptr)
	{
		ShipSelectionWidget3->InitShipSelectionWidget(
			TEXT("SHIP 03"),
			TEXT("GREEN"),
			FLinearColor(0.165132f, 0.768151f, 0.287441f),
			2);

		if (UButton* LaunchButton = ShipSelectionWidget3->GetLaunchButton())
		{
			LaunchButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnShipSelectionLaunchButton3Hovered);
		}
	}

	if (ShipSelectionWidget4 != nullptr)
	{
		ShipSelectionWidget4->InitShipSelectionWidget(
			TEXT("SHIP 04"),
			TEXT("PURPLE"),
			FLinearColor(0.361307f, 0.165132f, 1.0f),
			3);

		if (UButton* LaunchButton = ShipSelectionWidget4->GetLaunchButton())
		{
			LaunchButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnShipSelectionLaunchButton4Hovered);
		}
	}

	if (ShipSelectionWidget5 != nullptr)
	{
		ShipSelectionWidget5->InitShipSelectionWidget(
			TEXT("SHIP 05"),
			TEXT("RED"),
			FLinearColor(0.445201f, 0.005182f, 0.029557f),
			4);

		if (UButton* LaunchButton = ShipSelectionWidget5->GetLaunchButton())
		{
			LaunchButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnShipSelectionLaunchButton5Hovered);
		}
	}
}

void UPlayerShipSelectScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShipSelectionWidget1 != nullptr &&
		ShipSelectionWidget2 != nullptr &&
		ShipSelectionWidget3 != nullptr &&
		ShipSelectionWidget4 != nullptr &&
		ShipSelectionWidget5 != nullptr)
	{
		if (UButton* LaunchButton = ShipSelectionWidget1->GetLaunchButton())
		{
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Left, BackButton);
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget2->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
		}

		if (UButton* LaunchButton = ShipSelectionWidget2->GetLaunchButton())
		{
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget1->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget3->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
		}

		if (UButton* LaunchButton = ShipSelectionWidget3->GetLaunchButton())
		{
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget2->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget4->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
		}

		if (UButton* LaunchButton = ShipSelectionWidget4->GetLaunchButton())
		{
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget3->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget5->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
		}

		if (UButton* LaunchButton = ShipSelectionWidget5->GetLaunchButton())
		{
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget4->GetLaunchButton());
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Right, BackButton);
			LaunchButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
		}
	}
	
	if (BackButton != nullptr)
	{
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, ShipSelectionWidget1->GetLaunchButton());
		BackButton->SetNavigationRuleExplicit(EUINavigation::Left, ShipSelectionWidget5->GetLaunchButton());
		BackButton->SetNavigationRuleExplicit(EUINavigation::Right, ShipSelectionWidget1->GetLaunchButton());
	}
}

void UPlayerShipSelectScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	SetColorShiftForButton(BackButton, LinearColor);
	if (SelectYourShipTextBlock != nullptr)
	{
		SelectYourShipTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	if (ShipSelectionWidget1 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget1->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget2 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget2->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget3 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget3->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget4 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget4->GetLaunchButton(), LinearColor);
	}

	if (ShipSelectionWidget5 != nullptr)
	{
		SetColorShiftForButton(ShipSelectionWidget5->GetLaunchButton(), LinearColor);
	}
}

void UPlayerShipSelectScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnShipSelectBackClicked.Broadcast();
}

void UPlayerShipSelectScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}

void UPlayerShipSelectScreen::OnShipSelectionLaunchButton1Hovered()
{
	SetKeyboardFocusForWidgetLaunchButton(ShipSelectionWidget1);
}

void UPlayerShipSelectScreen::OnShipSelectionLaunchButton2Hovered()
{
	SetKeyboardFocusForWidgetLaunchButton(ShipSelectionWidget2);
}

void UPlayerShipSelectScreen::OnShipSelectionLaunchButton3Hovered()
{
	SetKeyboardFocusForWidgetLaunchButton(ShipSelectionWidget3);
}

void UPlayerShipSelectScreen::OnShipSelectionLaunchButton4Hovered()
{
	SetKeyboardFocusForWidgetLaunchButton(ShipSelectionWidget4);
}

void UPlayerShipSelectScreen::OnShipSelectionLaunchButton5Hovered()
{
	SetKeyboardFocusForWidgetLaunchButton(ShipSelectionWidget5);
}

void UPlayerShipSelectScreen::SetKeyboardFocusForWidgetLaunchButton(UShipSelectionWidget* const ShipSelectionWidget)
{
	if (ShipSelectionWidget != nullptr)
	{
		if (UButton* LaunchButton = ShipSelectionWidget->GetLaunchButton())
		{
			LaunchButton->SetKeyboardFocus();
		}
	}
}
