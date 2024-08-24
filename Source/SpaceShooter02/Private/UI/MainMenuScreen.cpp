// Copyright 2024 Richard Skala

#include "UI/MainMenuScreen.h"

//#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"


namespace MainMenuScreen
{
	static const TCHAR* VOTestAssetPath = TEXT("/Game/Sounds/A_VO_Test_01");
}

UMainMenuScreen::UMainMenuScreen()
{
	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Get the Test VO asset
		//ConstructorHelpers::FObjectFinderOptional<USoundBase> VOTestAssetFinder(MainMenuScreen::VOTestAssetPath);
		//VOTestSound = VOTestAssetFinder.Get();
	}
}

void UMainMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayButton != nullptr)
	{
		PlayButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPlayButtonClicked);
		PlayButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnPlayButtonHovered);
		//PlayButton->OnUnhovered.AddUniqueDynamic(this, &ThisClass::OnPlayButtonUnhovered);
		PlayButton->SetNavigationRuleExplicit(EUINavigation::Down, HighScoresButton);
	}

	if (HighScoresButton != nullptr)
	{
		HighScoresButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHighScoresButtonClicked);
		HighScoresButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnHighScoreButtonHovered);
		HighScoresButton->SetNavigationRuleExplicit(EUINavigation::Up, PlayButton);
		HighScoresButton->SetNavigationRuleExplicit(EUINavigation::Down, OptionsButton);
	}

	if (OptionsButton != nullptr)
	{
		OptionsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnOptionsButtonClicked);
		OptionsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnOptionsButtonHovered);
		OptionsButton->SetNavigationRuleExplicit(EUINavigation::Up, HighScoresButton);
		OptionsButton->SetNavigationRuleExplicit(EUINavigation::Down, ExitButton);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitButtonClicked);
		ExitButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnExitButtonHovered);
		ExitButton->SetNavigationRuleExplicit(EUINavigation::Up, OptionsButton);
	}

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);
		//CreditsButton->SetNavigationRuleExplicit(EUINavigation::Right, PlayButton);
	}

	if (VersionText != nullptr)
	{
		FText GameVersionString = FText::FromString(USpaceShooterGameInstance::GetGameVersionString());
		VersionText->SetText(GameVersionString);
	}
}

void UMainMenuScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (MainMenuTitleText != nullptr)
	{
		MainMenuTitleText->SetColorAndOpacity(FSlateColor(LinearColor));
	}
	
	SetColorShiftForButton(PlayButton, LinearColor);
	SetColorShiftForButton(OptionsButton, LinearColor);
	SetColorShiftForButton(ExitButton, LinearColor);
	SetColorShiftForButton(CreditsButton, LinearColor);
	SetColorShiftForButton(HighScoresButton, LinearColor);

}

void UMainMenuScreen::OnPlayButtonClicked()
{
	USpaceShooterMenuController::OnMainMenuPlayClicked.Broadcast();
}

void UMainMenuScreen::OnOptionsButtonClicked()
{
	USpaceShooterMenuController::OnMainMenuOptionsClicked.Broadcast();
}

void UMainMenuScreen::OnExitButtonClicked()
{
	UE_LOG(LogMenus, Log, TEXT("Quitting game from MainMenu..."));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMainMenuScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnMainMenuCreditsClicked.Broadcast();
}

void UMainMenuScreen::OnHighScoresButtonClicked()
{
	USpaceShooterMenuController::OnMainMenuHighScoreClicked.Broadcast();
}

void UMainMenuScreen::OnPlayButtonHovered()
{
	if (PlayButton != nullptr)
	{
		PlayButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::OnOptionsButtonHovered()
{
	if (OptionsButton != nullptr)
	{
		OptionsButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::OnExitButtonHovered()
{
	if (ExitButton != nullptr)
	{
		ExitButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		//CreditsButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::OnHighScoreButtonHovered()
{
	if (HighScoresButton != nullptr)
	{
		HighScoresButton->SetKeyboardFocus();
	}
}
