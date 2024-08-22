// Copyright 2024 Richard Skala

#include "UI/OptionsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "UI/SpaceShooterMenuController.h"

void UOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CreditsButton != nullptr)
	{
		CreditsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonClicked);
		CreditsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnCreditsButtonHovered);
		CreditsButton->SetNavigationRuleExplicit(EUINavigation::Down, StatsButton);
	}

	if (StatsButton != nullptr)
	{
		StatsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStatsButtonClicked);
		StatsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnStatsButtonHovered);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Up, CreditsButton);
		StatsButton->SetNavigationRuleExplicit(EUINavigation::Down, SoundEffectsOnOffButton);
	}

	if (SoundEffectsOnOffButton != nullptr)
	{
		SoundEffectsOnOffButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSoundEffectsOnOffButtonClicked);
		SoundEffectsOnOffButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnSoundEffectsOnOffButtonHovered);
		SoundEffectsOnOffButton->SetNavigationRuleExplicit(EUINavigation::Up, StatsButton);
		SoundEffectsOnOffButton->SetNavigationRuleExplicit(EUINavigation::Down, MusicSelectButton);
	}

	if (MusicSelectButton != nullptr)
	{
		MusicSelectButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnMusicSelectButtonClicked);
		MusicSelectButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnMusicSelectButtonHovered);
		MusicSelectButton->SetNavigationRuleExplicit(EUINavigation::Up, SoundEffectsOnOffButton);
		MusicSelectButton->SetNavigationRuleExplicit(EUINavigation::Down, ClearScoresButton);
	}

	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonClicked);
		ClearScoresButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnClearScoresButtonHovered);
		ClearScoresButton->SetNavigationRuleExplicit(EUINavigation::Up, MusicSelectButton);
		ClearScoresButton->SetNavigationRuleExplicit(EUINavigation::Down, ClearStatsButton);
	}

	if (ClearStatsButton != nullptr)
	{
		ClearStatsButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearStatsButtonClicked);
		ClearStatsButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnClearStatsButtonHovered);
		ClearStatsButton->SetNavigationRuleExplicit(EUINavigation::Up, ClearScoresButton);
		ClearStatsButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, ClearStatsButton);
	}
}

void UOptionsScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// Sound Effect Selection
		if (SoundEffectsOnOffButtonTextBlock != nullptr)
		{
			bool bSoundEffectsEnabled = GameInstance->GetSoundEffectsEnabled();
			FString SoundEffectEnabledString = bSoundEffectsEnabled ? "*ON*" : "*OFF*";
			SoundEffectsOnOffButtonTextBlock->SetText(FText::FromString(FString::Printf(TEXT("SOUND FX\n%s"), *SoundEffectEnabledString)));
		}

		// Music Selection
		if (MusicSelectButtonTextBlock != nullptr)
		{
			//Track1 = 0, // Index 0 into music list
			//Track2 = 1, // Index 1 into music list
			//Track3 = 2, // Index 2 into music list
			//MusicOff = 3, // Gameplay music will not play
			//Random = 4, // Randomly select a gameplay music track

			uint8 MusicSelection = GameInstance->GetMusicSelection();

			FString MusicSelectionString = "";
			switch (MusicSelection)
			{
				case 0: MusicSelectionString = "*TRACK 1*"; break;
				case 1: MusicSelectionString = "*TRACK 2*"; break;
				case 2: MusicSelectionString = "*TRACK 3*"; break;
				case 3: MusicSelectionString = "*OFF*"; break;
				case 4: MusicSelectionString = "*RANDOM*"; break;
				default: break;
			}
			MusicSelectButtonTextBlock->SetText(FText::FromString(FString::Printf(TEXT("MUSIC\n%s"), *MusicSelectionString)));
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
	SetColorShiftForButton(ClearScoresButton, LinearColor);
	SetColorShiftForButton(StatsButton, LinearColor);
	SetColorShiftForButton(ClearStatsButton, LinearColor);
	SetColorShiftForButton(SoundEffectsOnOffButton, LinearColor);
	SetColorShiftForButton(MusicSelectButton, LinearColor);
}

void UOptionsScreen::OnCreditsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenCreditsClicked.Broadcast();
}

void UOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenBackClicked.Broadcast();
}

void UOptionsScreen::OnClearScoresButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenClearScoresClicked.Broadcast();
}

void UOptionsScreen::OnStatsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenStatsClicked.Broadcast();
}

void UOptionsScreen::OnClearStatsButtonClicked()
{
	USpaceShooterMenuController::OnOptionsScreenClearStatsClicked.Broadcast();
}

void UOptionsScreen::OnSoundEffectsOnOffButtonClicked()
{
	USpaceShooterMenuController::OnOptionsSoundEffectClicked.Broadcast();
}

void UOptionsScreen::OnMusicSelectButtonClicked()
{
	USpaceShooterMenuController::OnOptionsMusicSelectClicked.Broadcast();
}

void UOptionsScreen::OnCreditsButtonHovered()
{
	if (CreditsButton != nullptr)
	{
		CreditsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnClearScoresButtonHovered()
{
	if (ClearScoresButton != nullptr)
	{
		ClearScoresButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnStatsButtonHovered()
{
	if (StatsButton != nullptr)
	{
		StatsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnClearStatsButtonHovered()
{
	if (ClearStatsButton != nullptr)
	{
		ClearStatsButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnSoundEffectsOnOffButtonHovered()
{
	if (SoundEffectsOnOffButton != nullptr)
	{
		SoundEffectsOnOffButton->SetKeyboardFocus();
	}
}

void UOptionsScreen::OnMusicSelectButtonHovered()
{
	if (MusicSelectButton != nullptr)
	{
		MusicSelectButton->SetKeyboardFocus();
	}
}
