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

		// When user presses down, highlight the Exit Button
		PlayButton->SetNavigationRuleExplicit(EUINavigation::Down, ExitButton);

		// When user presses left, highlight the Credits Button
		PlayButton->SetNavigationRuleExplicit(EUINavigation::Left, CreditsButton);

		PlayButton->SetNavigationRuleExplicit(EUINavigation::Right, HighScoresButton);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitButtonClicked);
		ExitButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnExitButtonHovered);

		ExitButton->SetNavigationRuleExplicit(EUINavigation::Up, PlayButton);
		ExitButton->SetNavigationRuleExplicit(EUINavigation::Left, CreditsButton);
		ExitButton->SetNavigationRuleExplicit(EUINavigation::Right, HighScoresButton);
	}

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);

		// When user presses right, highlight the Play button
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Right, PlayButton);
	}

	if (HighScoresButton != nullptr)
	{
		HighScoresButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHighScoresButtonClicked);
		HighScoresButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnHighScoreButtonHovered);
		
		HighScoresButton->SetNavigationRuleExplicit(EUINavigation::Left, PlayButton);
	}

	if (VersionText != nullptr)
	{
		FText GameVersionString = FText::FromString(USpaceShooterGameInstance::GetGameVersionString());
		VersionText->SetText(GameVersionString);
	}

	// Play the VO test sound
	//UGameplayStatics::PlaySound2D(GetWorld(), VOTestSound, 1.0f, 1.0f);
}

void UMainMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();

	// Set Play button as default keyboard focus for this screen
	if (PlayButton != nullptr)
	{
		PlayButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	// HACK workaround to force keyboard focus if all buttons lose focus.
	// This will occur if the user clicks the mouse outside of any button.
	if (PlayButton != nullptr && ExitButton != nullptr && CreditsButton != nullptr && HighScoresButton != nullptr)
	{
		if (!PlayButton->HasKeyboardFocus()
			&& !ExitButton->HasKeyboardFocus()
			&& !CreditsButton->HasKeyboardFocus()
			&& !HighScoresButton->HasKeyboardFocus())
		{
			UE_LOG(LogTemp, Warning, TEXT("UMainMenuScreen::NativeTick - No buttons have keyboard focus. Forcing to PlayButton"));
			PlayButton->SetKeyboardFocus();
		}
	}
}

void UMainMenuScreen::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

void UMainMenuScreen::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
}

void UMainMenuScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (MainMenuTitleText != nullptr)
	{
		MainMenuTitleText->SetColorAndOpacity(FSlateColor(LinearColor));
	}
	
	SetColorShiftForButton(PlayButton, LinearColor);
	SetColorShiftForButton(ExitButton, LinearColor);
	SetColorShiftForButton(CreditsButton, LinearColor);
	SetColorShiftForButton(HighScoresButton, LinearColor);
}

void UMainMenuScreen::OnPlayButtonClicked()
{
	USpaceShooterMenuController::OnMainMenuPlayClicked.Broadcast();
}

void UMainMenuScreen::OnExitButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("Quitting game from MainMenu..."));
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
		CreditsButton->SetKeyboardFocus();
	}
}

void UMainMenuScreen::OnHighScoreButtonHovered()
{
	if (HighScoresButton != nullptr)
	{
		HighScoresButton->SetKeyboardFocus();
	}
}
