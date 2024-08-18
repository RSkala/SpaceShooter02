// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnAnimController.generated.h"

UCLASS(Abstract, Blueprintable)
class SPACESHOOTER02_API USpawnAnimController : public UObject
{
	GENERATED_BODY()

public:
	void InitSpawnAnimPool();
	void ResetSpawnAnimPool();
	class ASpawnAnimBase* GetInactiveSpawnAnim();

private:
	class ASpawnAnimBase* CreateAndAddNewSpawnAnim(TSubclassOf<class ASpawnAnimBase> SpawnAnimClass);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class ASpawnAnimBase>> SpawnAnimClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class ASpawnAnimBase>> SpawnAnimPool;

private:
	static constexpr int32 MAX_SPAWN_ANIMS = 200;
};

