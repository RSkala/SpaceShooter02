// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"

#include "PoolActor.h"

#include "SpawnAnimBase.generated.h"

UCLASS(Abstract)
class SPACESHOOTER02_API ASpawnAnimBase : public APoolActor
{
	GENERATED_BODY()
	
public:	
	ASpawnAnimBase();
	virtual void Tick(float DeltaTime) override;
	virtual void ActivatePoolObject() override;
	virtual void DeactivatePoolObject() override;

protected:
	virtual void BeginPlay() override;

	virtual FVector GetInactivePoolObjectPosition() const override;

	UFUNCTION()
	void OnSpawnAnimationFinished();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> SpawnAnimFlipbookComp;

	static const FVector InactivePosition;
};
