// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "PickupItemBase.h"
#include "PickupItemSatelliteWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSatelliteWeaponPickedUpDelegateSignature);

UCLASS(Blueprintable)
class SPACESHOOTER02_API APickupItemSatelliteWeapon : public APickupItemBase
{
	GENERATED_BODY()

protected:
	virtual void HandlePlayerPickup() override;

public:
	static FSatelliteWeaponPickedUpDelegateSignature OnSatelliteWeaponPickedUp;
};
