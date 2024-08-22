// Copyright 2024 Richard Skala

#include "AudioController.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundWave.h"

void UAudioController::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Ensure each music track is set to looping (should have already been done in the asset)
		for (USoundWave* MusicSoundWave : GameplayMusicSoundWaveObjects)
		{
			MusicSoundWave->bLooping = true;
			//MusicSoundWave->bRequiresStopFade = true;
		}
	}
}

void UAudioController::PlayGameplayMusic(EMusicSelection MusicSelection)
{
	if (GameplayMusicSoundWaveObjects.Num() <= 0)
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
		MusicTrackIndex = FMath::RandRange(0, GameplayMusicSoundWaveObjects.Num() - 1);
	}
	else
	{
		MusicTrackIndex = static_cast<uint8>(MusicSelection);
	}

	if (ensure(MusicTrackIndex < GameplayMusicSoundWaveObjects.Num()))
	{
		USoundWave* MusicToPlay = GameplayMusicSoundWaveObjects[MusicTrackIndex];
		if (MusicToPlay != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYING MUSIC TRACK: %s"), *MusicToPlay->GetName());
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
