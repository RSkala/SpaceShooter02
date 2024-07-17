// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDeathDelegateSignature, FVector, EnemyDeathPosition);

UCLASS(Abstract)
class SPACESHOOTER02_API AEnemyBase : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_LOG_CATEGORY_CLASS(LogEnemy, Log, All)

public:	
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	void DestroyEnemy();
	void SetTarget(TSoftObjectPtr<AActor> InTargetActor);

protected:
	virtual void BeginPlay() override;
	virtual void MoveTowardsTarget(float DeltaTime);

public:
	UPROPERTY(BlueprintAssignable)
	FEnemyDeathDelegateSignature OnEnemyDeath;

protected:
	// --- Components ---

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

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
	TObjectPtr<class USoundBase> EnemyDeathSound; // TODO: Move to global place, so this is not duplicated
	

	// TODO:
	// * StopDistance
	// * DeathSound
	// * Death particle
};
