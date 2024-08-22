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

void UAudioController::PlayGameplayMusic()
{
	if (GameplayMusicSoundWaveObjects.Num() <= 0)
	{
		return;
	}

	// First stop any currently playing music
	StopGameplayMusicImmediately();

	// Randomly select a music track to play
	int32 RandomIdx = FMath::RandRange(0, GameplayMusicSoundWaveObjects.Num() - 1);
	USoundWave* MusicToPlay = GameplayMusicSoundWaveObjects[RandomIdx];
	if (MusicToPlay != nullptr)
	{
		CurrentGameplayMusic = UGameplayStatics::SpawnSound2D(GetWorld(), MusicToPlay, MusicVolume);
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
