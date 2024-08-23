// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
//#include "AudioEnums.generated.h" // Uncomment this if weird compile errors start to appear https://forums.unrealengine.com/t/enum-in-separate-files/151772/4

// ------------------------------------
// Music
// ------------------------------------

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

// ------------------------------------
// Sound Effects
// ------------------------------------

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

// ------------------------------------
// VO
// ------------------------------------
UENUM(BlueprintType)
enum class EMenuSoundVO : uint8
{
	Credits,
	GameOver,
	GoodLuck,
	SelectShip,
	Title,
	WelcomeBack,
	HighScores,

	NumMenuVOSounds
};
ENUM_RANGE_BY_COUNT(EMenuSoundVO, EMenuSoundVO::NumMenuVOSounds);

// Enum for tracking whether or not a sound VO was played
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESoundVOPlayed : uint8
{
	None = (0x0) UMETA(Hidden),
	CreditsVOPlayed = 1 << 0,
	GameOverVOPlayed = 1 << 2,
	GoodLuckVOPlayed = 1 << 3,
	SelectShipVOPlayed = 1 << 4,
	TitleVOPlayed = 1 << 5,
	WelcomeBackVOPlayed = 1 << 6,
	HighScoresVOPlayed = 1 << 7
};
ENUM_CLASS_FLAGS(ESoundVOPlayed);



