// Copyright 2024 Richard Skala

#include "PickupItemScoreMultiplier.h"

#include "SpaceShooterGameState.h"

FScoreMultiplierPickedUpDelegateSignature APickupItemScoreMultiplier::OnScoreMultiplierPickedUp;

void APickupItemScoreMultiplier::HandlePlayerPickup()
{
	OnScoreMultiplierPickedUp.Broadcast(ScoreMultiplierValue);
}
