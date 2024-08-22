// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AudioController.generated.h"

UENUM(BlueprintType)
enum class EMusicSelection : uint8 // An enum MUST be uint8 if using a BlueprintType
{
	Track1 = 0, // Index 0 into music list
	Track2 = 1, // Index 1 into music list
	Track3 = 2, // Index 2 into music list
	MusicOff = 3, // Gameplay music will not play
	Random = 4, // Randomly select a gameplay music track
	NumMusicTracks UMETA(Hidden)
};
ENUM_RANGE_BY_FIRST_AND_LAST(EMusicSelection, EMusicSelection::Track1, EMusicSelection::Random);

UCLASS(Abstract, Blueprintable)
class SPACESHOOTER02_API UAudioController : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

	// Music
	void PlayGameplayMusic(EMusicSelection MusicSelection);
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
