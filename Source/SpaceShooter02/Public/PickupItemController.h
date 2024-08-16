// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PickupItemController.generated.h"

UCLASS(Blueprintable)
class SPACESHOOTER02_API UPickupItemController : public UObject
{
	GENERATED_BODY()

public:
	void InitScoreMultiplierPool();
	void ResetScoreMultiplierPool();
	class APickupItemScoreMultiplier* GetInactiveScoreMultiplier();

private:
	class APickupItemScoreMultiplier* CreateAndAddNewScoreMultiplier();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class APickupItemScoreMultiplier> ScoreMultiplierClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class APickupItemScoreMultiplier>> ScoreMultiplierPool;

	static constexpr int32 MAX_SCORE_MULTIPLIERS = 100;
};
