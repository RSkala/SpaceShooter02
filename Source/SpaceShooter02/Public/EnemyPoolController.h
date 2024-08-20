// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyPoolController.generated.h"

// Handles different pools of enemies
UCLASS(Blueprintable)
class SPACESHOOTER02_API UEnemyPoolController : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	void InitEnemyPools();
	void ResetEnemyPools();
	class AEnemyBase* GetRandomEnemy();

private:
	// List of enemy classes to create pools from
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AEnemyBase>> EnemyClasses;
	
	// List of enemy pool containers. Number of pools containers will be exactly the number of enemy classes.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class UEnemyPoolContainer>> EnemyPoolContainers;

	static constexpr int32 MAX_ENEMIES_PER_POOL = 50;
};
