// Copyright 2024 Richard Skala

#include "PickupItemScoreMultiplier.h"

#include "PaperSpriteComponent.h"

#include "SpaceShooterGameState.h"

FScoreMultiplierPickedUpDelegateSignature APickupItemScoreMultiplier::OnScoreMultiplierPickedUp;

void APickupItemScoreMultiplier::UpdateLifetime(float DeltaTime)
{
	Super::UpdateLifetime(DeltaTime);

	if (!IsActorBeingDestroyed() && PaperSpriteComp != nullptr)
	{
		float PercentAlive = TimeAlive / LifeTimeSeconds;

		// When this pickup has reached greater than 50% of its lifetime, change color
		if (PercentAlive >= 0.5f)
		{
			if (PercentAlive <= 0.8f)
			{
				// greater than 50%, less than 80%
				//PaperSpriteComp->SetSpriteColor(FLinearColor::Yellow);
			}
			else
			{
				// greater than 80%
				//PaperSpriteComp->SetSpriteColor(FLinearColor::Red);
			}
		}
	}
}

void APickupItemScoreMultiplier::HandlePlayerPickup()
{
	OnScoreMultiplierPickedUp.Broadcast(ScoreMultiplierValue);
}
