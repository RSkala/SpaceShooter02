// Copyright 2024 Richard Skala

#include "EnemySpawner.h"

#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

#include "EnemyBase.h"
#include "EnemyPoolController.h"
#include "ExplosionBase.h"
#include "ExplosionSpriteController.h"
#include "PlayerShipPawn.h"
#include "SpaceShooterGameState.h"
#include "SpawnAnimBase.h"
#include "SpawnAnimController.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemySpawner, Log, All)

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSpawning(DeltaTime);
}

void AEnemySpawner::SetExplosionSpriteController(UExplosionSpriteController* InExplosionSpriteController)
{
	ExplosionSpriteController = InExplosionSpriteController;
}

void AEnemySpawner::SetSpawnAnimController(USpawnAnimController* InSpawnAnimController)
{
	SpawnAnimController = InSpawnAnimController;
}

void AEnemySpawner::SetEnemyPoolController(UEnemyPoolController* InEnemyPoolController)
{
	EnemyPoolController = InEnemyPoolController;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Get the game state
	SpaceShooterGameState = Cast<ASpaceShooterGameState>(UGameplayStatics::GetGameState(GetWorld()));

	UE_CLOG(
		!bSpawningEnabledDebug,
		LogEnemySpawner,
		Warning,
		TEXT("Enemy Spawing is DISABLED. Ensure this spawning re-enabled before committing the Enemy Spawner blueprint"));

	// Temp: Get a reference to the player ship to use as spawned enemy target - TODO: Handle on delegate
	PlayerShipPawn = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerShipPawn::StaticClass());
	if (PlayerShipPawn == nullptr)
	{
		UE_LOG(LogEnemySpawner, Warning, TEXT("%s - PlayerShipPawn not found"), ANSI_TO_TCHAR(__FUNCTION__));
	}

	// Notify the spawner when gameplay starts
	ASpaceShooterGameState::OnGameStarted.AddUniqueDynamic(this, &ThisClass::OnGameStarted);

	// Notify the spawner when an enemy dies
	AEnemyBase::OnEnemyDeath.AddUniqueDynamic(this, &ThisClass::OnEnemyDeath);
}

void AEnemySpawner::UpdateSpawning(float DeltaTime)
{
	// Exit if spawning is disabled
	if (!bSpawningEnabled || !bSpawningEnabledDebug)
	{
		return;
	}

	// Debug Spawn Radiuses
	if(bShowDebugSpawnRadius)
	{
		FVector EnemySpawnSourcePos = PlayerShipPawn != nullptr ? PlayerShipPawn->GetActorLocation() : FVector();
		FVector UpAxis = FVector(0.0f, 0.0f, 1.0f);
		FVector ForwardAxis = FVector(1.0f, 0.0f, 0.0f);

		DrawDebugCircle(GetWorld(), EnemySpawnSourcePos, SpawnDistanceFromPlayerMin, 25, FColor::Cyan, false, -1.0f, 0, 0.0f, UpAxis, ForwardAxis, false);
		DrawDebugCircle(GetWorld(), EnemySpawnSourcePos, SpawnDistanceFromPlayerMax, 25, FColor::Emerald, false, -1.0f, 0, 0.0f, UpAxis, ForwardAxis, false);
	}

	TimeSinceLastEnemySpawned += DeltaTime;
	if (TimeSinceLastEnemySpawned >= GetTimeBetweenSpawns())
	{
		// Time has elapsed since the last enemy was spawned. Spawn an enemy.

		// Get a random position to spawn the enemy using the range
		float RandomDistance = FMath::FRandRange(SpawnDistanceFromPlayerMin, SpawnDistanceFromPlayerMax);

		// Get a random position at the edge of the unit sphere
		FVector RandomUnitSphereEdgePos = FMath::VRand();

		// Discard the Y-component, then normalize the vector to "push" the position to the edge of a unit circle
		FVector2D RandomPosition = FVector2D(RandomUnitSphereEdgePos).GetSafeNormal();

		// Multiply by the random distance for the spawn position offset from the player
		RandomPosition *= RandomDistance;

		// Get the enemy spawn position
		FVector SourcePosition = PlayerShipPawn != nullptr ? PlayerShipPawn->GetActorLocation() : FVector();
		FVector EnemyPosition = SourcePosition + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);

		// Play a Spawn Animation at the enemy spawn position
		if (SpawnAnimController != nullptr)
		{
			ASpawnAnimBase* EnemySpawnAnim = SpawnAnimController->GetInactiveSpawnAnim();
			if (EnemySpawnAnim != nullptr)
			{
				float RandomRotation = FMath::FRandRange(0.0f, 360.0f);
				FRotator SpawnAnimRotation(RandomRotation, 0.0f, 0.0f); // Y rotation is Pitch
				FVector SpawnAnimPos = FVector(EnemyPosition.X, 0.2f, EnemyPosition.Z); // have spawn anim appear in front of enemy

				EnemySpawnAnim->SetActorLocationAndRotation(SpawnAnimPos, SpawnAnimRotation);
				EnemySpawnAnim->ActivatePoolObject();
			}
		}

		// Spawn the enemy
		if (EnemyPoolController != nullptr)
		{
			AEnemyBase* SpawnedEnemy = EnemyPoolController->GetRandomEnemy();
			if (SpawnedEnemy != nullptr)
			{
				// Set player as the enemy's target and place at the spawn position
				SpawnedEnemy->SetTarget(PlayerShipPawn);
				SpawnedEnemy->SetActorLocation(EnemyPosition);
				SpawnedEnemy->ActivatePoolObject();
			}
		}

		// Reset time since last spawn
		TimeSinceLastEnemySpawned = 0.0f;
	}
}

void AEnemySpawner::OnGameStarted()
{
	// Gameplay has started. Start enemy spawning.
	SetSpawningEnabled(true);
}

void AEnemySpawner::OnEnemyDeath(FVector EnemyDeathPosition, UNiagaraSystem* EnemyDeathEffect, USoundBase* EnemyDeathSound)
{
	//UE_LOG(LogEnemySpawner, Warning, TEXT("%s - EnemyDeathPosition: %s"), ANSI_TO_TCHAR(__FUNCTION__), *EnemyDeathPosition.ToString());

	UWorld* World = GetWorld();

	// Spawn an explosion at this enemy death position
	if (EnemyExplosionClasses.Num() > 0)
	{
		if (World != nullptr)
		{
			// Randomly select an enemy explosion type to spawn
			TSubclassOf<AExplosionBase> ExplosionClassToSpawn = EnemyExplosionClasses[FMath::RandRange(0, EnemyExplosionClasses.Num() - 1)];

			// Get a random rotation for the explosion for variety
			FRotator RandomExplosionRotation = FRotator(FMath::FRandRange(0.0f, 360.0f), 0.0f, 0.0f);

			// Create the explosion
			if (ExplosionSpriteController != nullptr)
			{
				AExplosionBase* ExplosionSprite = ExplosionSpriteController->GetRandomInactiveExplosionSprite();
				if (ExplosionSprite != nullptr)
				{
					ExplosionSprite->SetActorLocationAndRotation(EnemyDeathPosition, RandomExplosionRotation);
					ExplosionSprite->ActivatePoolObject();
				}
			}
		}
	}

	// Spawn an explosion particle at the enemy death position
	if(EnemyDeathEffect != nullptr)
	{
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnemyExplosionEffect.Get(), EnemyDeathPosition);
		FFXSystemSpawnParameters SpawnParams;
		SpawnParams.WorldContextObject = GetWorld();
		SpawnParams.SystemTemplate = EnemyDeathEffect;
		SpawnParams.Location = EnemyDeathPosition;
		SpawnParams.Rotation = FRotator::ZeroRotator;
		SpawnParams.Scale = FVector::OneVector;
		SpawnParams.bAutoDestroy = true;
		SpawnParams.bAutoActivate = true;
		SpawnParams.PoolingMethod = ToPSCPoolMethod(ENCPoolMethod::None);
		SpawnParams.bPreCullCheck = true;
		UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(SpawnParams);
	}

	// Stop the death/explosion sound if playing
	if (CurrentEnemyExplosionSound != nullptr)
	{
		CurrentEnemyExplosionSound->Stop();
	}
	CurrentEnemyExplosionSound = nullptr;

	// Adjust a random pitch and play the enemy death sound
	static const float ExplodeSoundPitchAdjust = 0.1f;
	float DeathSoundPitch = 1.0f + FMath::FRandRange(-ExplodeSoundPitchAdjust, ExplodeSoundPitchAdjust);
	CurrentEnemyExplosionSound = UGameplayStatics::SpawnSound2D(World, EnemyDeathSound, DeathSoundPitch);
}

float AEnemySpawner::GetTimeBetweenSpawns() const
{
	float CurTimeBetweenSpawns = FallbackTimeBetweenSpawns;
	if (SpaceShooterGameState.IsValid())
	{
		CurTimeBetweenSpawns = SpaceShooterGameState.Get()->GetTimeBetweenSpawns();
	}
	return CurTimeBetweenSpawns;
}

#if WITH_EDITOR
bool AEnemySpawner::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void AEnemySpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property being changed
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawner, SpawnDistanceFromPlayerMin))
	{
		// Ensure the spawn distance min is never greater than spawn distance from player max
		if (SpawnDistanceFromPlayerMin > SpawnDistanceFromPlayerMax)
		{
			// The min is greater than the max. Set the max to this new min.
			SpawnDistanceFromPlayerMax = SpawnDistanceFromPlayerMin;
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawner, SpawnDistanceFromPlayerMax))
	{
		// Ensure the spawn distance max is never less than spawn distance from player min
		if (SpawnDistanceFromPlayerMax < SpawnDistanceFromPlayerMin)
		{
			// The max is less than the min. Set the min to this new max.
			SpawnDistanceFromPlayerMin = SpawnDistanceFromPlayerMax;
		}
	}
}
#endif // WITH_EDITOR
