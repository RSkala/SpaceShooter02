// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// Class for handling enemy spawning. TODO: Make this into Subsystem
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
	void OnEnemyDeath(FVector EnemyDeathPosition);

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// List of enemies that can be spawned.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class AEnemyBase>> EnemyClasses;

	// Number of seconds that need to elapse before spawning another enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "Seconds"))
	float TimeBetweenSpawns = 1.0f;

	// Distance to spawn from player (min)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "50", ClampMax = "5000", UIMin = "50", UIMax = "5000"))
	float SpawnDistanceFromPlayerMin = 200.0f;

	// Distance to spawn from player (max)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "50", ClampMax = "5000", UIMin = "50", UIMax = "5000"))
	float SpawnDistanceFromPlayerMax = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class AExplosionBase>> EnemyExplosionClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class APlayerShipPawn> PlayerShipPawn;

	// Enable / Disable enemy spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawningEnabled = true;

	// TODO:
	// * DifficultySpikeInterval -- every X enemy defeated, increase difficulty
	// * get player reference, pass to enemies
	
	// Last time an enemy was spawned
	float TimeSinceLastEnemySpawned = 0.0f;
};
