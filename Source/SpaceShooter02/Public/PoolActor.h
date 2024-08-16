// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PoolObject.h"

#include "PoolActor.generated.h"

UCLASS(Abstract)
class SPACESHOOTER02_API APoolActor : public AActor, public IPoolObject
{
	GENERATED_BODY()
	
public:	
	APoolActor();
	virtual void Tick(float DeltaTime) override;

	// IPoolObject
	virtual void ActivatePoolObject() override;
	virtual void DeactivatePoolObject() override;
	bool IsPoolObjectActive() const { return bIsPoolObjectActive; }

protected:
	virtual void BeginPlay() override;
	virtual void UpdateLifetime(float DeltaTime);

public:
	static const FVector InactivePoolActorPosition;

protected:
	// Whether this object is active and visible in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPoolObjectActive = false;

	// How long this actor stays alive / active
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTimeSeconds = MAX_flt - 1.0f;

	// How long this object has been alive / active
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeAlive = 0.0f;
};
