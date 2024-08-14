// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// Class for handling enemy spawning
UCLASS()
class SPACESHOOTER02_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;
	void SetSpawningEnabled(bool bInSpawningEnabled) { bSpawningEnabled = bInSpawningEnabled; }

protected:
	virtual void BeginPlay() override;
	void UpdateSpawning(float DeltaTime);

	UFUNCTION()
	void OnGameStarted();

	UFUNCTION()
	void OnEnemyDeath(FVector EnemyDeathPosition, class UNiagaraSystem* EnemyDeathEffect, class USoundBase* EnemyDeathSound);

	float GetTimeBetweenSpawns() const;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// List of enemies that can be spawned.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class AEnemyBase>> EnemyClasses;

	// Default Time Between Spawns. Only used if GameInstance ptr is invalid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (Units = "Seconds"))
	float FallbackTimeBetweenSpawns = 1.0f;

	// Distance to spawn from player (min)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "50", ClampMax = "5000", UIMin = "50", UIMax = "5000"))
	float SpawnDistanceFromPlayerMin = 200.0f;

	// Distance to spawn from player (max)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "50", ClampMax = "5000", UIMin = "50", UIMax = "5000"))
	float SpawnDistanceFromPlayerMax = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowDebugSpawnRadius = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class AExplosionBase>> EnemyExplosionClasses;

	// Used for setting the player as a target
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;

	// Enable / Disable enemy spawning
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSpawningEnabled = true; // Set during game

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawningEnabledDebug = true; // Set manually for debugging

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> EnemyExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class ASpawnAnimBase>> EnemySpawnAnimClasses;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> CurrentEnemyExplosionSound;

	UPROPERTY()
	TWeakObjectPtr<class ASpaceShooterGameState> SpaceShooterGameState;
	
	// Last time an enemy was spawned
	float TimeSinceLastEnemySpawned = 0.0f;
};
