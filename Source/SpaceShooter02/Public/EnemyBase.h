// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"

#include "PoolActor.h"

#include "EnemyBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDeathDelegateSignature, FVector, EnemyDeathPosition, class UNiagaraSystem*, EnemyDeathEffect);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyDeathDelegateSignature, FVector, EnemyDeathPosition, class UNiagaraSystem*, EnemyDeathEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FEnemyDeathDelegateSignature,
	FVector, EnemyDeathPosition,
	class UNiagaraSystem*, EnemyDeathEffect,
	bool, bKilledFromBoost);

UCLASS(Abstract)
class SPACESHOOTER02_API AEnemyBase : public APoolActor
{
	GENERATED_BODY()

public:
	DECLARE_LOG_CATEGORY_CLASS(LogEnemy, Log, All)

public:	
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

	// APoolActor / IPoolObject
	virtual void ActivatePoolObject() override;
	virtual void DeactivatePoolObject() override;

	void DestroyEnemy(bool bDestroyedFromBoost = false);
	void SetTarget(TSoftObjectPtr<AActor> InTargetActor);

protected:
	virtual void BeginPlay() override;
	virtual void MoveTowardsTarget(float DeltaTime);
	virtual void OnSpawnDelayTimerElapsed();

public:
	//UPROPERTY(BlueprintAssignable)
	static FEnemyDeathDelegateSignature OnEnemyDeath;

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	// --- Lifetime / Movement ---
	// NOTE: In order to use Units, they must be listed in the FParseCandidates in UnitConversion.cpp

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 1000.0f;

	// By default, the enemy should move towards its target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<AActor> TargetActor;

	// If true, enemy will rotate towards its target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateToFaceTarget = true;

	// --- Effects ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UNiagaraSystem> EnemyExplosionEffect;

	// Flag for "delaying" the enemy before appearing onscreen (timed with spawn animations)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSpawning = true;

	UPROPERTY()
	FTimerHandle SpawnDelayTimerHandle;
};
