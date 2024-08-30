// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UENUM(BlueprintType)
enum class EEnemySpawnType : uint8
{
	RadiusAroundPlayer,
	BoxInsideGameorders,

	NumSpawnTypes UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EEnemySpawnType, EEnemySpawnType::NumSpawnTypes);

// Class for handling enemy spawning
UCLASS()
class SPACESHOOTER02_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;
	void SetSpawningEnabled(bool bInSpawningEnabled) { bSpawningEnabled = bInSpawningEnabled; }
	void SetExplosionSpriteController(class UExplosionSpriteController* InExplosionSpriteController);
	void SetSpawnAnimController(class USpawnAnimController* InSpawnAnimController);
	void SetEnemyPoolController(class UEnemyPoolController* InEnemyPoolController);

protected:
	virtual void BeginPlay() override;
	void UpdateSpawning(float DeltaTime);

	UFUNCTION()
	void OnGameStarted();

	UFUNCTION()
	void OnEnemyDeath(FVector EnemyDeathPosition, class UNiagaraSystem* EnemyDeathEffect, bool bKilledFromBoost);

	float GetTimeBetweenSpawns() const;

	FVector GetRandomEnemySpawnPosition() const;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Spawn type to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemySpawnType EnemySpawnType;

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

	UPROPERTY()
	TWeakObjectPtr<class ASpaceShooterGameState> SpaceShooterGameState;

	UPROPERTY()
	TWeakObjectPtr<class UExplosionSpriteController> ExplosionSpriteController;

	UPROPERTY()
	TWeakObjectPtr<class USpawnAnimController> SpawnAnimController;

	UPROPERTY()
	TWeakObjectPtr<class UEnemyPoolController> EnemyPoolController;
	
	// Last time an enemy was spawned
	float TimeSinceLastEnemySpawned = 0.0f;
};
