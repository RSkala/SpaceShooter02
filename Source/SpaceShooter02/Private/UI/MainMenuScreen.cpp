// Copyright 2024 Richard Skala

#include "UI/MainMenuScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameState.h"

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

	// NOTE: If I were to load a level, use: UGameplayStatics::OpenLevel() or UGameplayStatics::OpenLevelBySoftObjectPtr

	if (UWorld* World = GetWorld())
	{
		if (ASpaceShooterGameState* SpaceShooterGameState = World->GetGameState<ASpaceShooterGameState>())
		{
			SpaceShooterGameState->StartGame();
		}
	}
}

void UMainMenuScreen::OnExitButtonClicked()
{
	UE_LOG(LogMenus, Warning, TEXT("Quitting game from MainMenu..."));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
