// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AudioController.generated.h"

UCLASS(Abstract, Blueprintable)
class SPACESHOOTER02_API UAudioController : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

	// Music
	void PlayGameplayMusic();
	void FadeOutGameplayMusic();
	void StopGameplayMusicImmediately();

private:

	// --- Music ---

	// List of gameplay music tracks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class USoundWave>> GameplayMusicSoundWaveObjects;

	// How long it takes to fade out the currently playing music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MusicFadeTime = 2.0f;

	// Volume at which to play music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MusicVolume = 0.9f;

	// Currently playing music track. Used for fading and stopping.
	UPROPERTY()
	TObjectPtr<class UAudioComponent> CurrentGameplayMusic;
};
