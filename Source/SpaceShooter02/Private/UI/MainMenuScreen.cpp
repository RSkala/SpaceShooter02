// Copyright 2024 Richard Skala

#include "UI/MainMenuScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"

void UMainMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayButton != nullptr)
	{
		PlayButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPlayButtonClicked);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitButtonClicked);
	}

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
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
}

void UMainMenuScreen::OnPlayButtonClicked()
{
	UE_LOG(LogMenus, Log, TEXT("OnPlayButtonClicked"));
	USpaceShooterMenuController::OnMainMenuPlayClicked.Broadcast();
}

void UMainMenuScreen::OnExitButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("Quitting game from MainMenu..."));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMainMenuScreen::OnCreditsButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("OnCreditsButtonClicked"));
}

