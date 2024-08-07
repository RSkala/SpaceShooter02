// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAnimBase.generated.h"

UCLASS(Abstract)
class SPACESHOOTER02_API ASpawnAnimBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnAnimBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSpawnAnimationFinished();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> SpawnAnimFlipbookComp;
};
