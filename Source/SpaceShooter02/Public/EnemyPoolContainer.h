// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyPoolContainer.generated.h"

UCLASS(Blueprintable)
class SPACESHOOTER02_API UEnemyPoolContainer : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	void InitEnemyPool(TSubclassOf<class AEnemyBase> InEnemyClass, int32 MaxNumEnemies);
	void ResetEnemyPool();
	AEnemyBase* GetInactiveEnemy();

private:
	AEnemyBase* CreateAndAddEnemyToPool();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AEnemyBase>> EnemyPool;

	UPROPERTY()
	TSubclassOf<class AEnemyBase> EnemyClass;
};
