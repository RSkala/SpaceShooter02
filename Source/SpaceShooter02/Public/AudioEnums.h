// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
//#include "AudioEnums.generated.h" // Uncomment this if weird compile errors start to appear https://forums.unrealengine.com/t/enum-in-separate-files/151772/4

// Enum for Music Tracks
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


// Enum for Sound Effects
UENUM(BlueprintType)
enum class ESoundEffect : uint8
{
	// UI
	ButtonClick,

	// Player
	ShipShootSound,
	ShipBoostSound,
	MultiplierPickupSound,
	PowerupEarnedSound,
	PowerupLevelUpSound,
	PowerupTimeAddedSound,
	ShipExplosionSound,

	// Enemy
	EnemyDeathSound,
	
	NumSounds
};
ENUM_RANGE_BY_COUNT(ESoundEffect, ESoundEffect::NumSounds);



