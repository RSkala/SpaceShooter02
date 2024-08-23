// Copyright 2024 Richard Skala

#include "AudioController.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"

DEFINE_LOG_CATEGORY_STATIC(LogAudioController, Log, All)

void UAudioController::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Ensure each music track is set to looping (should have already been done in the asset)
		for (USoundWave* MusicSoundWave : GameplayMusicTracks)
		{
			MusicSoundWave->bLooping = true;
			//MusicSoundWave->bRequiresStopFade = true;
		}
	}
}

void UAudioController::PlayGameplayMusic(EMusicSelection MusicSelection)
{
	if (GameplayMusicTracks.Num() <= 0)
	{
		return;
	}

	// First stop any currently playing music
	StopGameplayMusicImmediately();

	// Do not play music if music is disabled
	if (MusicSelection == EMusicSelection::MusicOff)
	{
		return;
	}

	int32 MusicTrackIndex = 0;
	if (MusicSelection == EMusicSelection::Random)
	{
		// Randomly select a music track to play
		MusicTrackIndex = FMath::RandRange(0, GameplayMusicTracks.Num() - 1);
	}
	else
	{
		MusicTrackIndex = static_cast<uint8>(MusicSelection);
	}

	if (ensure(MusicTrackIndex < GameplayMusicTracks.Num()))
	{
		USoundWave* MusicToPlay = GameplayMusicTracks[MusicTrackIndex];
		if (MusicToPlay != nullptr)
		{
			UE_LOG(LogAudioController, Log, TEXT("Playing Music Track: %s"), *MusicToPlay->GetName());
			CurrentGameplayMusic = UGameplayStatics::SpawnSound2D(GetWorld(), MusicToPlay, MusicVolume);
		}
	}
}

void UAudioController::FadeOutGameplayMusic()
{
	if (CurrentGameplayMusic != nullptr)
	{
		CurrentGameplayMusic->FadeOut(MusicFadeTime, 0.0f);
		CurrentGameplayMusic = nullptr;
	}
}

void UAudioController::StopGameplayMusicImmediately()
{
	if (CurrentGameplayMusic != nullptr)
	{
		CurrentGameplayMusic->Stop();
	}
	CurrentGameplayMusic = nullptr;
}

void UAudioController::PlaySound(ESoundEffect SoundEffect)
{
	switch (SoundEffect)
	{
		// --- UI ---
		case ESoundEffect::ButtonClick:
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
			break;

		// --- Gameplay ---
		case ESoundEffect::ShipShootSound:
			//float ShootSoundPitch = 1.0f + FMath::FRandRange(-ShootSoundPitchAdjust, ShootSoundPitchAdjust);
			//UGameplayStatics::PlaySound2D(GetWorld(), PlayerShootSound, ShootSoundVolume, ShootSoundPitch);
			break;

		case ESoundEffect::ShipBoostSound:
			//const float ShootSoundPitch = 1.0f + FMath::FRandRange(-ShootSoundPitchAdjust, ShootSoundPitchAdjust);
			UGameplayStatics::PlaySound2D(GetWorld(), ShipBoostSound, 1.0f, 1.0f);
			break;

		case ESoundEffect::MultiplierPickupSound:
		{
			StopSound(CurrentMultiplierPickedupSound);

			// Adjust a random pitch and play the pickup item sounds
			const float PitchAdjust = 0.1f;
			float SoundPitch = 1.0f + FMath::FRandRange(-PitchAdjust, PitchAdjust);
			CurrentMultiplierPickedupSound = UGameplayStatics::SpawnSound2D(GetWorld(), MultiplierPickupSound, 0.9f, SoundPitch);
		}
		break;

		case ESoundEffect::PowerupEarnedSound:
			StopSound(CurrentPowerupEarnedSound);
			CurrentPowerupEarnedSound = UGameplayStatics::SpawnSound2D(GetWorld(), PowerupEarnedSound);
			break;

		case ESoundEffect::PowerupLevelUpSound:
			StopSound(CurrentPowerupLevelUpSound);
			CurrentPowerupLevelUpSound = UGameplayStatics::SpawnSound2D(GetWorld(), PowerupLevelUpSound);
			break;

		case ESoundEffect::PowerupTimeAddedSound:
			StopSound(CurrentPowerupTimeAddedSound);
			CurrentPowerupTimeAddedSound = UGameplayStatics::SpawnSound2D(GetWorld(), PowerupTimeAddedSound);
			break;

		case ESoundEffect::ShipExplosionSound:
			UGameplayStatics::PlaySound2D(GetWorld(), ShipExplosionSound);
			break;

		case ESoundEffect::EnemyDeathSound:
		{
			StopSound(CurrentEnemyDeathSound);

			// Adjust a random pitch and play the enemy death sound
			const float ExplodeSoundPitchAdjust = 0.1f;
			float DeathSoundPitch = 1.0f + FMath::FRandRange(-ExplodeSoundPitchAdjust, ExplodeSoundPitchAdjust);
			CurrentEnemyDeathSound = UGameplayStatics::SpawnSound2D(GetWorld(), EnemyDeathSound, 1.0f, DeathSoundPitch);
		}
		break;

		default:
			break;
	}
}

void UAudioController::PlayMenuVO(EMenuSoundVO MenuSoundVO)
{
	switch (MenuSoundVO)
	{
		case EMenuSoundVO::Credits:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::CreditsVOPlayed))
			{
				SelectAndPlayRandomVO(CreditsVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::CreditsVOPlayed);
			}
			break;

		case EMenuSoundVO::GameOver:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::GameOverVOPlayed))
			{
				SelectAndPlayRandomVO(GameOverVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::GameOverVOPlayed);
			}
			break;

		case EMenuSoundVO::GoodLuck:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::GoodLuckVOPlayed))
			{
				SelectAndPlayRandomVO(GoodLuckVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::GoodLuckVOPlayed);
			}
			else if (!HasSoundVOBeenPlayed(ESoundVOPlayed::WelcomeBackVOPlayed))
			{
				SelectAndPlayRandomVO(WelcomeBackVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::WelcomeBackVOPlayed);
			}
			break;

		case EMenuSoundVO::SelectShip:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::SelectShipVOPlayed))
			{
				SelectAndPlayRandomVO(SelectShipVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::SelectShipVOPlayed);
			}
			break;

		case EMenuSoundVO::Title:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::TitleVOPlayed))
			{
				SelectAndPlayRandomVO(TitleVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::TitleVOPlayed);
			}
			break;

		case EMenuSoundVO::WelcomeBack:
			// Handled in GoodLuck case
			/*if (!HasSoundVOBeenPlayed(ESoundVOPlayed::WelcomeBackVOPlayed))
			{
				SelectAndPlayRandomVO(WelcomeBackVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::WelcomeBackVOPlayed);
			}*/
			break;

		case EMenuSoundVO::HighScores:
			if (!HasSoundVOBeenPlayed(ESoundVOPlayed::HighScoresVOPlayed))
			{
				SelectAndPlayRandomVO(HighScoreVOSounds);
				SetSoundVOPlayed(ESoundVOPlayed::HighScoresVOPlayed);
			}
			break;

		default:
			break;
	}
}

void UAudioController::StopSound(TObjectPtr<class UAudioComponent>& AudioComponent)
{
	if (AudioComponent != nullptr)
	{
		AudioComponent->Stop();
	}
	AudioComponent = nullptr;
}

void UAudioController::SelectAndPlayRandomVO(TArray<TSoftObjectPtr<USoundBase>> SoundVOArray)
{
	if (SoundVOArray.Num() <= 0)
	{
		return;
	}

	int32 RandomIdx = FMath::RandRange(0, SoundVOArray.Num() - 1);
	TSoftObjectPtr SoundVOToPlayPtr = SoundVOArray[RandomIdx];
	//USoundBase* SoundVOToPlay = SoundVOToPlayPtr.Get(); // This will not always be valid! Use LoadSynchronous() instead.
	USoundBase* SoundVOToPlay = SoundVOToPlayPtr.LoadSynchronous();

	// Stop the current VO sound so they don't overlap
	if (CurrentVOSound != nullptr)
	{
		CurrentVOSound->Stop();
		CurrentVOSound = nullptr;
	}
	CurrentVOSound = UGameplayStatics::SpawnSound2D(GetWorld(), SoundVOToPlay);
}

bool UAudioController::HasSoundVOBeenPlayed(ESoundVOPlayed SoundVOPlayed) const
{
	return (SoundVOPlayedFlags & (uint8)SoundVOPlayed) != 0;
	//return true; // disable VO temp
}

void UAudioController::SetSoundVOPlayed(ESoundVOPlayed SoundVOPlayed)
{
	SoundVOPlayedFlags |= (uint8)SoundVOPlayed;
}


