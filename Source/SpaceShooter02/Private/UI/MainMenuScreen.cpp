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
	if (PlayButton != nullptr && ExitButton != nullptr && CreditsButton != nullptr && HighScoresButton != nullptr && OptionsButton != nullptr)
	{
		if (!PlayButton->HasKeyboardFocus()
			&& !ExitButton->HasKeyboardFocus()
			&& !CreditsButton->HasKeyboardFocus()
			&& !HighScoresButton->HasKeyboardFocus()
			&& !OptionsButton->HasKeyboardFocus())
		{
			//UE_LOG(LogTemp, Warning, TEXT("UMainMenuScreen::NativeTick - No buttons have keyboard focus. Forcing to PlayButton"));
			//PlayButton->SetKeyboardFocus();
		}
	}
}

void UMainMenuScreen::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

void UMainMenuScreen::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	// Potential alternate fix for mouse losing focus when clicking outside any widget:
	// https://forums.unrealengine.com/t/my-buttons-lose-keyboard-focus-if-a-mouse-is-clicked/1150836/10
	// https://forums.unrealengine.com/t/common-ui-and-keyboard-only-no-mouse/1234750/4

	//TWeakPtr<SWidget> PreviousWidget = PreviousFocusPath.IsValid() ? PreviousFocusPath.GetLastWidget() : nullptr;
	//FName PreviousWidgetTypeName = PreviousWidget.IsValid() ? PreviousWidget.Pin().Get()->GetType() : NAME_None;

	FName NewWidgetTypeName = NAME_None;
	if (NewWidgetPath.IsValid()) // Need to check IsValid, otherwise GetLastWidget() could error out (happens on viewport resize, e.g. via F11 in PIE)
	{
		TSharedRef<SWidget> NewWidget = NewWidgetPath.GetLastWidget();
		NewWidgetTypeName = NewWidget.Get().GetType();
	}

	//UE_LOG(LogTemp, Warning, TEXT("----------------------------------"));
	//UE_LOG(LogTemp, Warning, TEXT("PreviousWidgetTypeName: %s"), *PreviousWidgetTypeName.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("NewWidgetTypeName:      %s"), *NewWidgetTypeName.ToString());

	if (InFocusEvent.GetCause() == EFocusCause::Mouse && (NewWidgetTypeName == FName("SPIEViewport") || NewWidgetTypeName == FName("SViewport")))
	{
		if (PlayButton != nullptr)
		{
			UE_LOG(LogMenus, Warning, TEXT("User clicking on a viewport. Forcing keyboard focus to %s"), *PlayButton->GetName());
			PlayButton->SetKeyboardFocus();
			return;
		}
	}

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
