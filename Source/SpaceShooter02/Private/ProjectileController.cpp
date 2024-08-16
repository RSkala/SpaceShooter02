// Copyright 2024 Richard Skala

#include "ProjectileController.h"

#include "ProjectileBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectileController, Log, All)

void UProjectileController::InitProjectilePool()
{
	for (int i = 0; i < MAX_PROJECTILES; ++i)
	{
		CreateAndAddNewProjectile();
	}
}

void UProjectileController::ResetProjectilePool()
{
	// Iterate through the entire projectile pool and deactivate each
	for (AProjectileBase* const Projectile : ProjectilePool)
	{
		if (Projectile != nullptr)
		{
			Projectile->DeactivatePoolObject();
		}
	}
}

AProjectileBase* UProjectileController::GetInactiveProjectile()
{
	// Get first inactive projectile
	AProjectileBase* InactiveProjectile = nullptr;
	for (AProjectileBase* Projectile : ProjectilePool)
	{
		if (Projectile != nullptr && !Projectile->IsPoolObjectActive())
		{
			// Found an inactive projectile
			InactiveProjectile = Projectile;
			break;
		}
	}

	// FindByPredicate returns a pointer which is making it super annoying to assign so just use a normal for-loop as above
	/*TObjectPtr<AProjectileBase>* InactiveProjectile = ProjectilePool.FindByPredicate([](TObjectPtr<AProjectileBase> Projectile)
	{
		return Projectile != nullptr && !Projectile->IsProjectileActive();
	});*/

	if (InactiveProjectile == nullptr)
	{
		UE_LOG(LogProjectileController, Warning, TEXT("No available projectiles in the pool. Increase the pool size."));
		InactiveProjectile = CreateAndAddNewProjectile();
	}
	return InactiveProjectile;
}

AProjectileBase* UProjectileController::CreateAndAddNewProjectile()
{
	AProjectileBase* NewProjectile = nullptr;
	if (ensure(ProjectileClass != nullptr))
	{
		FActorSpawnParameters ProjectileSpawnParameters;
		ProjectileSpawnParameters.Name = TEXT("Projectile");
		//ProjectileSpawnParameters.Instigator = this;
		ProjectileSpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		ProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (UWorld* World = GetWorld())
		{
			NewProjectile = World->SpawnActor<AProjectileBase>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, ProjectileSpawnParameters);
			if (NewProjectile != nullptr)
			{
				ProjectilePool.Add(NewProjectile);
			}
			else
			{
				UE_LOG(LogProjectileController, Warning, TEXT("Failed to create projectile."));
			}
		}
	}
	return NewProjectile;
}
