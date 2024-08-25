// Copyright 2024 Richard Skala

#include "UI/SoundOptionsScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "SpaceShooterGameInstance.h"
#include "UI/SpaceShooterMenuController.h"

void USoundOptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MusicSelectButton != nullptr)
	{
		MusicSelectButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnMusicSelectButtonClicked);
		MusicSelectButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnMusicSelectButtonHovered);
		MusicSelectButton->SetNavigationRuleExplicit(EUINavigation::Up, BackButton);
		MusicSelectButton->SetNavigationRuleExplicit(EUINavigation::Down, SoundEffectsOnOffButton);
	}

	if (SoundEffectsOnOffButton != nullptr)
	{
		SoundEffectsOnOffButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSoundEffectsOnOffButtonClicked);
		SoundEffectsOnOffButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnSoundEffectsOnOffButtonHovered);
		SoundEffectsOnOffButton->SetNavigationRuleExplicit(EUINavigation::Up, MusicSelectButton);
		SoundEffectsOnOffButton->SetNavigationRuleExplicit(EUINavigation::Down, VOOnOffButton);
	}

	if (VOOnOffButton != nullptr)
	{
		VOOnOffButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnVOOnOffButton);
		VOOnOffButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnVOOnOffButtonHovered);
		VOOnOffButton->SetNavigationRuleExplicit(EUINavigation::Up, SoundEffectsOnOffButton);
		VOOnOffButton->SetNavigationRuleExplicit(EUINavigation::Down, BackButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackButtonClicked);
		BackButton->OnHovered.AddUniqueDynamic(this, &ThisClass::OnBackButtonHovered);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Up, VOOnOffButton);
		BackButton->SetNavigationRuleExplicit(EUINavigation::Down, MusicSelectButton);
	}
}

void USoundOptionsScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// Music Selection
		if (MusicSelectButtonTextBlock != nullptr)
		{
			EMusicSelection MusicSelection = GameInstance->GetMusicSelection();

			FString MusicSelectionString = "";
			switch (MusicSelection)
			{
				case EMusicSelection::Track1: MusicSelectionString = "*TRACK 1*"; break;
				case EMusicSelection::Track2: MusicSelectionString = "*TRACK 2*"; break;
				case EMusicSelection::Track3: MusicSelectionString = "*TRACK 3*"; break;
				case EMusicSelection::MusicOff: MusicSelectionString = "*OFF*"; break;
				case EMusicSelection::Random: MusicSelectionString = "*RANDOM*"; break;
				default: break;
			}
			MusicSelectButtonTextBlock->SetText(FText::FromString(FString::Printf(TEXT("MUSIC\n%s"), *MusicSelectionString)));
		}

		// Sound Effect Selection
		if (SoundEffectsOnOffButtonTextBlock != nullptr)
		{
			bool bSoundEffectsEnabled = GameInstance->GetSoundEffectsEnabled();
			FString SoundEffectEnabledString = bSoundEffectsEnabled ? "*ON*" : "*OFF*";
			SoundEffectsOnOffButtonTextBlock->SetText(FText::FromString(FString::Printf(TEXT("SOUND FX\n%s"), *SoundEffectEnabledString)));
		}

		// VO Selection
		if (VOOnOffButtonTextBlock != nullptr)
		{
			bool bVOEnabled = GameInstance->GetVOEnabled();
			FString VOEnabledString = bVOEnabled ? "*ON*" : "*OFF*";
			VOOnOffButtonTextBlock->SetText(FText::FromString(FString::Printf(TEXT("VOICE\n%s"), *VOEnabledString)));
		}
	}
}

void USoundOptionsScreen::OnColorShift(FLinearColor LinearColor)
{
	Super::OnColorShift(LinearColor);

	if (SoundsTitleTextBlock != nullptr)
	{
		SoundsTitleTextBlock->SetColorAndOpacity(FSlateColor(LinearColor));
	}

	SetColorShiftForButton(MusicSelectButton, LinearColor);
	SetColorShiftForButton(SoundEffectsOnOffButton, LinearColor);
	SetColorShiftForButton(VOOnOffButton, LinearColor);
	SetColorShiftForButton(BackButton, LinearColor);
}

void USoundOptionsScreen::OnMusicSelectButtonClicked()
{
	USpaceShooterMenuController::OnSoundScreenMusicSelectClicked.Broadcast();
}

void USoundOptionsScreen::OnSoundEffectsOnOffButtonClicked()
{
	USpaceShooterMenuController::OnSoundScreenSoundEffectClicked.Broadcast();
}

void USoundOptionsScreen::OnVOOnOffButton()
{
	USpaceShooterMenuController::OnSoundScreenVOClicked.Broadcast();
}

void USoundOptionsScreen::OnBackButtonClicked()
{
	USpaceShooterMenuController::OnSoundScreenBackClicked.Broadcast();
}

void USoundOptionsScreen::OnMusicSelectButtonHovered()
{
	if (MusicSelectButton != nullptr)
	{
		MusicSelectButton->SetKeyboardFocus();
	}
}

void USoundOptionsScreen::OnSoundEffectsOnOffButtonHovered()
{
	if (SoundEffectsOnOffButton != nullptr)
	{
		SoundEffectsOnOffButton->SetKeyboardFocus();
	}
}

void USoundOptionsScreen::OnVOOnOffButtonHovered()
{
	if (VOOnOffButton != nullptr)
	{
		VOOnOffButton->SetKeyboardFocus();
	}
}

void USoundOptionsScreen::OnBackButtonHovered()
{
	if (BackButton != nullptr)
	{
		BackButton->SetKeyboardFocus();
	}
}
