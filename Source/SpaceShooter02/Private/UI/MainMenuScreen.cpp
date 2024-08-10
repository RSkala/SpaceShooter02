// Copyright 2024 Richard Skala

#include "UI/MainMenuScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
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
		//PlayButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnPlayButtonHovered);
		//PlayButton->OnUnhovered.AddUniqueDynamic(this, &ThisClass::OnPlayButtonUnhovered);

		// When user presses down, highlight the Exit Button
		PlayButton->SetNavigationRuleExplicit(EUINavigation::Down, ExitButton);

		// When user presses left, highlight the Credits Button
		PlayButton->SetNavigationRuleExplicit(EUINavigation::Left, CreditsButton);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitButtonClicked);

		// When user presses up, highlight the Play Button
		ExitButton->SetNavigationRuleExplicit(EUINavigation::Up, PlayButton);

		// When user presses left, highlight the Credits Button
		ExitButton->SetNavigationRuleExplicit(EUINavigation::Left, CreditsButton);
	}

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);

		// When user presses right, highlight the Play button
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Right, PlayButton);
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
}

FNavigationReply UMainMenuScreen::NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

FNavigationReply UMainMenuScreen::NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& NavigationEvent)
{
	return Super::NativeOnNavigation(InGeometry, NavigationEvent);
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
