// Copyright 2024 Richard Skala

#include "PickupItemSatelliteWeapon.h"

FSatelliteWeaponPickedUpDelegateSignature APickupItemSatelliteWeapon::OnSatelliteWeaponPickedUp;

void APickupItemSatelliteWeapon::HandlePlayerPickup()
{
	OnSatelliteWeaponPickedUp.Broadcast();
}
