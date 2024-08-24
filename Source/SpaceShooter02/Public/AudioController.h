// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AudioEnums.h"

#include "AudioController.generated.h"

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
	void PauseGameplayMusic();
	void UnpauseGameplayMusic();

	// Sound Effects
	void PlaySound(ESoundEffect SoundEffect);

	// VO
	void PlayMenuVO(EMenuSoundVO MenuSoundVO);

private:
	// Sound Effects
	void StopSound(TObjectPtr<class UAudioComponent>& AudioComponent);

	// VO
	void SelectAndPlayRandomVO(ESoundVOPlayed SoundVOPlayed, TArray<TSoftObjectPtr<USoundBase>> SoundVOArray);
	void SelectAndPlayRandomVO(TArray<TSoftObjectPtr<USoundBase>> SoundVOArray);
	bool HasSoundVOBeenPlayed(ESoundVOPlayed SoundVOPlayed) const;
	void SetSoundVOPlayed(ESoundVOPlayed SoundVOPlayed);

	// Pause/Unpause
	UFUNCTION() void OnRequestPauseGame();
	UFUNCTION() void OnRequestUnpauseGame();
	UFUNCTION() void OnRequestSelfDestruct();

private:
	// -------------
	// --- Debug ---
	// -------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = true))
	bool bDisableMenuVO = false;

	// -------------
	// --- Music ---
	// -------------

	// List of gameplay music tracks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class USoundWave>> GameplayMusicTracks;

	// How long it takes to fade out the currently playing music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (AllowPrivateAccess = true))
	float MusicFadeTime = 2.0f;

	// Volume at which to play music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (AllowPrivateAccess = true))
	float MusicVolume = 0.9f;

	// Currently playing music track. Used for fading and stopping.
	UPROPERTY()
	TObjectPtr<class UAudioComponent> CurrentGameplayMusic;

	// ----------
	// --- UI ---
	// ----------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> ButtonClickSound;

	// ----------------
	// --- Gameplay ---
	// ----------------
	
	// Played when the player shoots (if shoot sounds enabled)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> PlayerShootSound;

	// Played when the player starts a "Boost"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> ShipBoostSound;

	// Played when the player picks up a score multiplier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> MultiplierPickupSound;

	// Played when the player earned a powerup (after collecting enough pickups)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> PowerupEarnedSound;

	// Played when the player earns a powerup level (after picking up an item while powered up)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> PowerupLevelUpSound;

	// Played when the player earns more time (after picking up an item while powered up)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> PowerupTimeAddedSound;

	// Played when the player's ship is destroyed (game over)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> ShipExplosionSound;

	// Played when an enemy is defeated
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> EnemyDeathSound;

	// -----------------------------------------------------
	// Sound Tracking variables

	UPROPERTY() TObjectPtr<class UAudioComponent> CurrentMultiplierPickedupSound;
	UPROPERTY() TObjectPtr<class UAudioComponent> CurrentPowerupEarnedSound;
	UPROPERTY() TObjectPtr<class UAudioComponent> CurrentPowerupLevelUpSound;
	UPROPERTY() TObjectPtr<class UAudioComponent> CurrentPowerupTimeAddedSound;
	UPROPERTY() TObjectPtr<class UAudioComponent> CurrentEnemyDeathSound;

	// ---------------
	// --- Menu VO ---
	// ---------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> CreditsVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> GameOverVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> GoodLuckVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> SelectShipVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> TitleVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> WelcomeBackVOSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, Category = "Menu VO"))
	TArray<TSoftObjectPtr<class USoundBase>> HighScoreVOSounds;

	// --- Flags for limiting VO sound playing ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BitMask, BitmaskEnum = "/Script/SpaceShooter02.ESoundVOPlayed", AllowPrivateAccess = true))
	uint8 SoundVOPlayedFlags;

	// Currently playing VO sound
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAudioComponent> CurrentVOSound;
};
