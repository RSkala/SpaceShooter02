// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "PickupItemBase.h"
#include "PickupItemScoreMultiplier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreMultiplierPickedUpDelegateSignature, int32, ScoreMultiplierValue);

UCLASS(Blueprintable)
class SPACESHOOTER02_API APickupItemScoreMultiplier : public APickupItemBase
{
	GENERATED_BODY()

protected:
	virtual void UpdateLifetime(float DeltaTime) override;
	virtual void HandlePlayerPickup() override;

public:
	static FScoreMultiplierPickedUpDelegateSignature OnScoreMultiplierPickedUp;

protected:
	// Amount to add to the current score multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ScoreMultiplierValue = 1;
};
