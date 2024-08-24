// Copyright 2024 Richard Skala

#include "PlayerShipPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"
//#include "TimerManager.h"

#include "EnemyBase.h"
#include "EnemySpawner.h"
#include "PickupItemScoreMultiplier.h"
#include "ProjectileBase.h"
#include "SpaceShooterGameInstance.h"
#include "SpaceShooterGameState.h"
#include "UI/SpaceShooterMenuController.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawn, Warning, All)
DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawnInput, Warning, All)
DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawnMovement, Log, All)

FPlayerShipSpawnedDelegateSignature APlayerShipPawn::OnPlayerShipSpawned;
FPlayerShipDestroyedDelegateSignature APlayerShipPawn::OnPlayerShipDestroyed;
FPlayerPowerupTimerUpdatedDelegateSignature APlayerShipPawn::OnPlayerPowerupTimerUpdated;
FPlayerDashUpdatedDelegateSignature APlayerShipPawn::OnPlayerDashUpdated;

namespace
{
	static const TCHAR* DefaultPlayerShipSpritePath = TEXT("/Game/Sprites/PlayerShips/SPR_PlayerShip_2");
	static const TCHAR* DefaultPlayerShipExhaustFlipbookPath = TEXT("/Game/Sprites/ShipExhaust/PFB_Ship_Exhaust_1");
}

// Sets default values
APlayerShipPawn::APlayerShipPawn()
{
	UE_LOG(LogPlayerShipPawn, Verbose, TEXT("APlayerShipPawn::APlayerShipPawn - %s"), *GetName());

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetupAttachment(RootComponent);
	PaperSpriteComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Set the default ship sprite (for the CDO only)
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultPlayerShipSpriteFinder(DefaultPlayerShipSpritePath);
		PaperSpriteComp->SetSprite(DefaultPlayerShipSpriteFinder.Get());
	}
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1000.0f; // While armlength doesn't matter for orthographic, set far enough away so as to not be in the way
	SpringArmComp->bDoCollisionTest = false; // Ensure the SpringArm does not collide with anything in the world (so it isn't "pushed")
	SpringArmComp->SetAbsolute(false, true, false); // Set the SpringArm rotation to Absolute, so its rotation is not affected by the ship in any way
	SpringArmComp->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f)); // Set the rotation so it is looking down the Y-axis towards the XZ plane (Note: Yaw is Z-axis rotation)
	//SpringArmComp->bEnableCameraLag = true; // Note: Camera lag doesn't work with Orthographic
	//SpringArmComp->CameraLagSpeed = 8.0f;
	//SpringArmComp->bDrawDebugLagMarkers = false;
	//SpringArmComp->TargetArmLength = 1500.0f; // Better value for Perspective projection (depending on FOV)

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComp->OrthoWidth = 3840.0f;
	//CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
	//CameraComp->FieldOfView = 115.0f;
	//CameraComp->bConstrainAspectRatio = true; // TODO: Test this value to see if it has any positive or negative effects

	// Fire Points
	//TObjectPtr<class USceneComponent> FirePointRotatorComp;

	FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
	FirePointComp->SetupAttachment(RootComponent);

	FirePointRotatorComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointRotatorComp"));
	FirePointRotatorComp->SetupAttachment(RootComponent);

	FirePointComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp1"));
	FirePointComp1->SetupAttachment(FirePointRotatorComp);

	FirePointComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp2"));
	FirePointComp2->SetupAttachment(FirePointRotatorComp);

	ShipExhaustFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipExhaustFlipbookComp"));
	ShipExhaustFlipbookComp->SetupAttachment(RootComponent);
	ShipExhaustFlipbookComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Set the default player ship exhaust flipbook (for the CDO only)
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DefaultPlayerShipExhaustFlipbookFinder(DefaultPlayerShipExhaustFlipbookPath);
		ShipExhaustFlipbookComp->SetFlipbook(DefaultPlayerShipExhaustFlipbookFinder.Get());
	}

	ShipExhaustParticleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShipExhaustParticleComp"));
	ShipExhaustParticleComp->SetupAttachment(RootComponent);

	DashShieldSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DashShieldSpriteComp"));
	DashShieldSpriteComp->SetupAttachment(RootComponent);
	DashShieldSpriteComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	DashExhaustParticleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashExhaustParticleComp"));
	DashExhaustParticleComp->SetupAttachment(RootComponent);

	DashReadyFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DashReadyFlipbookComp"));
	DashReadyFlipbookComp->SetupAttachment(RootComponent);
	DashReadyFlipbookComp->SetLooping(false);
	DashReadyFlipbookComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// --- Satellite Weapon Components ---

	SatelliteWeaponRotatorComp = CreateDefaultSubobject<USceneComponent>(TEXT("SatelliteWeaponRotatorComp"));
	SatelliteWeaponRotatorComp->SetupAttachment(RootComponent);
	SatelliteWeaponRotatorComp->SetAbsolute(false, true, false); // Set the rotation to Absolute, so it is not affected by player rotation

	SatelliteWeaponSprite1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SatelliteWeaponSprite1"));
	SatelliteWeaponSprite1->SetupAttachment(SatelliteWeaponRotatorComp);
	SatelliteWeaponSprite1->SetAbsolute(false, true, false);
	SatelliteWeaponSprite1->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SatelliteWeaponSprite2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SatelliteWeaponSprite2"));
	SatelliteWeaponSprite2->SetupAttachment(SatelliteWeaponRotatorComp);
	SatelliteWeaponSprite2->SetAbsolute(false, true, false);
	SatelliteWeaponSprite2->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SatelliteWeaponSprite3 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SatelliteWeaponSprite3"));
	SatelliteWeaponSprite3->SetupAttachment(SatelliteWeaponRotatorComp);
	SatelliteWeaponSprite3->SetAbsolute(false, true, false);
	SatelliteWeaponSprite3->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SatelliteWeaponSprite4 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SatelliteWeaponSprite4"));
	SatelliteWeaponSprite4->SetupAttachment(SatelliteWeaponRotatorComp);
	SatelliteWeaponSprite4->SetAbsolute(false, true, false);
	SatelliteWeaponSprite4->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Effect attach point
	PowerupEffectAttachPoint = CreateDefaultSubobject<USceneComponent>("PowerupEffectAttachPoint");
	PowerupEffectAttachPoint->SetupAttachment(RootComponent);
	PowerupEffectAttachPoint->SetAbsolute(false, true, false); // Set rotation to absolute so the effects do not "spin" around when the player ship rotates
}

// Called every frame
void APlayerShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Update player ship from movement and direction inputs
	//UE_LOG(LogPlayerShipPawn, Log, TEXT("MovementDirection: %s"), *MovementDirection.ToString());
	//UE_LOG(LogPlayerShipPawn, Log, TEXT("AimingDirection:   %s"), *AimingDirection.ToString());

	// Show / Hide the ship's exhaust
	UpdateExhaust();

	// Update the ship movement from the movement input
	UpdateMovement(DeltaTime);
	UpdateGamepadAimFiring();

	// Increase the time since last shot
	TimeSinceLastShot += DeltaTime;

	// Update the rotation of the satellite weapon "rotator" component
	UpdateSatelliteWeaponRotation(DeltaTime);

	// Update powerup active
	if (PlayerHasPowerup())
	{
		PowerupActiveTimer -= DeltaTime;
		if (PowerupActiveTimer <= 0.0f)
		{
			PowerupTimerElapsed();
		}

		// Ensure PowerupActiveTimer doesn't go below zero
		PowerupActiveTimer = FMath::Max(0.0f, PowerupActiveTimer);

		// Notify that powerup time updated
		OnPlayerPowerupTimerUpdated.Broadcast(PowerupActiveTimer / PowerupActiveTime);
	}
}

// Called to bind functionality to input
void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::SetupPlayerInputComponent - %s"), *GetName());
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Keyboard Movement
		ensureAlways(InputActionKeyboardMove != nullptr);
		EnhancedInputComponent->BindAction(InputActionKeyboardMove, ETriggerEvent::Triggered, this, &ThisClass::KeyboardMoveTriggered);
		EnhancedInputComponent->BindAction(InputActionKeyboardMove, ETriggerEvent::Completed, this, &ThisClass::KeyboardMoveCompleted);
		
		// Bind Gamepad Movement
		ensureAlways(InputActionGamepadMove != nullptr);
		EnhancedInputComponent->BindAction(InputActionGamepadMove, ETriggerEvent::Triggered, this, &ThisClass::GamepadMoveTriggered);
		EnhancedInputComponent->BindAction(InputActionGamepadMove, ETriggerEvent::Completed, this, &ThisClass::GamepadMoveCompleted);

		// Bind Gamepad Aiming
		ensureAlways(InputActionGamepadAim != nullptr);
		EnhancedInputComponent->BindAction(InputActionGamepadAim, ETriggerEvent::Triggered, this, &ThisClass::GamepadAimTriggered);
		EnhancedInputComponent->BindAction(InputActionGamepadAim, ETriggerEvent::Completed, this, &ThisClass::GamepadAimCompleted);

		// Bind Firing/Shooting
		ensureAlways(InputActionFire != nullptr);
		//EnhancedInputComponent->BindAction(InputActionFire, ETriggerEvent::Started, this, &ThisClass::Fire); // Started: When the player first presses the Fire button
		EnhancedInputComponent->BindAction(InputActionFire, ETriggerEvent::Triggered, this, &ThisClass::Fire); // Triggered: When the player HOLDS down the Fire button

		// Bind Mouse Firing
		ensureAlways(InputActionMouseFire != nullptr);
		EnhancedInputComponent->BindAction(InputActionMouseFire, ETriggerEvent::Triggered, this, &ThisClass::MouseFire); // Triggered: When the player HOLDS down the MouseFire button

		// Bind Dash
		ensureAlways(InputActionDash != nullptr);
		EnhancedInputComponent->BindAction(InputActionDash, ETriggerEvent::Started, this, &ThisClass::InputDash);

		// Bind Pause
		ensureAlways(InputActionPause != nullptr);
		EnhancedInputComponent->BindAction(InputActionPause, ETriggerEvent::Started, this, &ThisClass::InputPause);

		// Bind Clean Pause
#if !UE_BUILD_SHIPPING // No clean pause in shipping builds
		ensureAlways(InputActionCleanPause != nullptr);
		EnhancedInputComponent->BindAction(InputActionCleanPause, ETriggerEvent::Started, this, &ThisClass::InputCleanPause);
#endif
	}
}

// Called when the game starts or when spawned
void APlayerShipPawn::BeginPlay()
{
	UE_LOG(LogPlayerShipPawn, Verbose, TEXT("APlayerShipPawn::BeginPlay - %s"), *GetName());
	Super::BeginPlay();

	// Get the ship's playercontroller 
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Ensure the mouse cursor is visible by default (Hide when the player touches gamepad controls)
		SetMouseCursorVisiblityFromInput(PlayerController, true);

		// Register Inputs (add mapping context) from the UEnhancedInputLocalPlayerSubsystem (which is accessed from the LocalPlayer)
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}

		// ---------------------------------------------------------------------
		// Note: Can also get the subsystem like this:
		/*if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		}*/
		// ---------------------------------------------------------------------
	}

	// Weapons / Projectiles
	FireRate = 1.0f / ProjectileShotsPerSecond;

	// Initialize "time since last shot" to the fire rate, so there is no delay on the very first shot
	TimeSinceLastShot = FireRate;

	// Randomly pick a ship sprite
	if (PlayerShipSprites.Num() > 0)
	{
		UPaperSprite* RandomSprite = PlayerShipSprites[FMath::RandRange(0, PlayerShipSprites.Num() - 1)];
		PaperSpriteComp->SetSprite(RandomSprite);
	}

	SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionOverlap);

	// Start ship exhaust particle deactivated
	if (ShipExhaustParticleComp != nullptr)
	{
		ShipExhaustParticleComp->Deactivate();
	}

	if (DashExhaustParticleComp != nullptr)
	{
		DashExhaustParticleComp->Deactivate();
	}

	if (DashReadyFlipbookComp != nullptr)
	{
		ensure(DashReadyFlipbookComp->GetFlipbook() != nullptr);

		// Subscribe to the OnFinishedPlaying delegate to be notified when the flipbook animation completed. bLooping MUST be false.
		DashReadyFlipbookComp->OnFinishedPlaying.AddUniqueDynamic(this, &ThisClass::OnDashReadyAnimationFinished);
		DashReadyFlipbookComp->SetHiddenInGame(true);
		bDashReadyAnimPlayed = true; // Ensure the dash animation doesn't play when first starting up a game
	}

	// Start the player disabled
	DisablePlayer();

	// Listen for Game Start and Game End
	ASpaceShooterGameState::OnGameStarted.AddUniqueDynamic(this, &ThisClass::OnGameStarted);
	ASpaceShooterGameState::OnGameEnded.AddUniqueDynamic(this, &ThisClass::OnGameEnded);

	// Listen for Player Ship selection
	USpaceShooterMenuController::OnPlayerShipSelected.AddUniqueDynamic(this, &ThisClass::OnPlayerShipSelected);

	// Listen for item pickup

	ASpaceShooterGameState::OnAddSatelliteWeapon.AddUniqueDynamic(this, &ThisClass::AddSatelliteWeapon);
	//ASpaceShooterGameState::OnPickupItemPercentChanged.AddUniqueDynamic(this, &ThisClass::PickupItemPercentChanged);
	APickupItemScoreMultiplier::OnScoreMultiplierPickedUp.AddUniqueDynamic(this, &ThisClass::OnScoreMultiplierPickedUp);

	// Start satellite weapons disabled
	DisableSatelliteWeapons();

	// Hide dash shield sprite
	HideDashShield();

	// Get the game state reference
	SpaceShooterGameState = Cast<ASpaceShooterGameState>(UGameplayStatics::GetGameState(GetWorld()));
	ensure(SpaceShooterGameState != nullptr);
}

void APlayerShipPawn::UpdateMovement(float DeltaTime)
{
	UE_LOG(LogPlayerShipPawnMovement, Verbose, TEXT("APlayerShipPawn::UpdateMovement: %s"), *MovementDirection.ToString());

	// =======================================================================
	// Handle Dash
	float ModifiedMoveSpeed = MoveSpeed;
	FVector2D ModifiedMovementDirection = MovementDirection;
	if (bIsDashing)
	{
		ModifiedMoveSpeed = MoveSpeed * DashMoveSpeedMultiplier;

		DashTimeElapsed += DeltaTime;
		if (DashTimeElapsed >= DashTime)
		{
			// Finished dashing
			bIsDashing = false;
			HideDashShield();

			// Shrink the collision sphere radius back to normal
			if (SphereComp != nullptr)
			{
				SphereComp->SetSphereRadius(PreDashCollisionSphereRadius, true);
			}

			// Disable dash exhaust particle
			if (DashExhaustParticleComp != nullptr)
			{
				DashExhaustParticleComp->Deactivate();
			}

			// Reset "dash ready" anim flag
			bDashReadyAnimPlayed = false;
		}
		else
		{
			// If the user has no input, force movement in its forward direction
			if (FMath::IsNearlyZero(MovementDirection.SquaredLength()))
			{
				FVector PlayerShipForward = GetActorUpVector(); // "Up" is Z-direction
				ModifiedMovementDirection = FVector2D(PlayerShipForward.X, PlayerShipForward.Z);
			}
		}
	}
	else
	{
		// Player is not dashing. Recharge the dash meter.
		DashRechargeTimeElapsed += DeltaTime;
		DashRechargeTimeElapsed = FMath::Min(DashRechargeTimeElapsed, DashRechargeTime);
		OnPlayerDashUpdated.Broadcast(DashRechargeTimeElapsed / DashRechargeTime);

		// When the dash recharge time has elapsed, play the "dash ready" animation
		if (DashRechargeTimeElapsed >= DashRechargeTime)
		{
			// Play dash ready flipbook animation
			if (DashReadyFlipbookComp != nullptr)
			{
				if (!bDashReadyAnimPlayed)
				{
					DashReadyFlipbookComp->SetHiddenInGame(false);
					DashReadyFlipbookComp->PlayFromStart();
					bDashReadyAnimPlayed = true;
				}
			}
		}
	}
	// =======================================================================

	if (ModifiedMovementDirection.SquaredLength() > 0.0f)
	{
		// Valid movement input - update the player ship position and rotation
		
		// --------------------------------------------------------
		// Update the ship position
		
		// Get player ship's current position
		FVector PlayerShipPosition = GetActorLocation();

		// Get the distance to move this frame using the movement direction
		// Movement formula, Distance = Rate * Time
		FVector2D MovementAmount2D = ModifiedMovementDirection * ModifiedMoveSpeed * DeltaTime;

		// Get the movement amount as a 3D vector, as SetActorLocation() uses FVector
		//FVector MovementAmount = FVector(MovementAmount2D.X, PlayerShipPosition.Y, MovementAmount2D.Y);
		FVector MovementAmount = FVector(MovementAmount2D.X, 0.0f, MovementAmount2D.Y);

		// Get the new ship position and set it
		//FVector NewShipPosition = PlayerShipPosition + MovementAmount;
		//SetActorLocation(NewShipPosition);

		// Line trace to check collision to create a new modified movment direction, depending on whether the player collided with walls.
		// Separating into two separate LineTraces allows the player to "slide" along the wall in a non-collided direction
		if (UWorld* World = GetWorld())
		{
			// Use the player ship's position as the line trace start position
			const FVector& TraceStartPos = GetActorLocation();

			// Check agains all static objects (TODO: Use custom collision object type, e.g. Walls)
			FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::InitType::AllStaticObjects);

			// Distance of the LineTrace. Adjust with the movement speed and deltatime
			//float CollisionDistance = NewMoveSpeed * DeltaTime * CollisionLineTraceOffset;
			float CollisionDistance = MoveSpeed * DeltaTime * CollisionLineTraceOffset;

			// Check collision against walls in the X movement direction
			TArray<FHitResult> HitResultsXDir;
			const FVector& TraceEndPosXDir = TraceStartPos + FVector(ModifiedMovementDirection.X, 0.0f, 0.0f).GetSafeNormal() * CollisionDistance;
			if (World->LineTraceMultiByObjectType(HitResultsXDir, TraceStartPos, TraceEndPosXDir, ObjectQueryParams))
			{
				// Successful line trace hit in the X direction. Zero out the X movement amount.
				MovementAmount.X = 0.0f;
			}
			
			// Check collision against walls in the Y movement direction (Z world direction)
			TArray<FHitResult> HitResultsYDir;
			const FVector& TraceEndPosYDir = TraceStartPos + FVector(0.0f, 0.0f, ModifiedMovementDirection.Y).GetSafeNormal() * CollisionDistance;
			if (World->LineTraceMultiByObjectType(HitResultsYDir, TraceStartPos, TraceEndPosYDir, ObjectQueryParams))
			{
				// Successful line trace hit in the Y direction. Zero out the Y movement amount.
				MovementAmount.Z = 0.0f;
			}

			if (bShowMovementCollisionDebug)
			{
				UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("CollisionDistance: %f"), CollisionDistance);
				DrawDebugLine(World, TraceStartPos, TraceEndPosXDir, FColor::Red);
				DrawDebugLine(World, TraceStartPos, TraceEndPosYDir, FColor::Green);
			}

			FVector AdjustedShipPosition = PlayerShipPosition + MovementAmount;
			SetActorLocation(AdjustedShipPosition);

			// TODO: Save for sweeps later
			//FHitResult SweepHitResult;
			//bool bActorLocationSet = SetActorLocation(AdjustedShipPosition, true, &SweepHitResult, ETeleportType::None);
		}


		/////////////////////////////////////////////////////////////////

		// ---------------------------------------------------------
		// Update the ship facing direction

		// Note/Todo: I probably can use 2D calculations instead:
		//float Dot2D = FVector2D::UnitY().Dot(MovementDirection); // FVector2d::DotProduct(FVector2D::UnitY(), MovementDirection);
		//float Acos2D = FMath::Acos(Dot2D);
		//float AngleDegrees2D = FMath::RadiansToDegrees(Acos2D);
		//float Cross2D = FVector2D::CrossProduct(FVector2D::UnitY(), MovementDirection); // Get the 2D "Wedge Product"
		//float AngleDirectionMultipler2D = Cross2D >= 0.0f ? 1.0f : -1.0f; // If result > 0, then direction is the left of the Up-Vector
		//AngleDegrees2D *= AngleDirectionMultipler2D;

		// Get the movement direction in 3D
		FVector MovementDirection3D = FVector(ModifiedMovementDirection.X, 0.0f, ModifiedMovementDirection.Y);

		// Get the angle between the world up vector (+Z axis) and the movement input direction
		float Dot = FVector::UnitZ().Dot(MovementDirection3D);
		float Acos = FMath::Acos(Dot);
		float AngleDegrees = FMath::RadiansToDegrees(Acos);

		FVector Cross = FVector::CrossProduct(FVector::UnitZ(), MovementDirection3D);
		//UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("Cross: %s"), *Cross.ToString());

		// Determine the rotation direction from the cross product (left-hand rule since Unreal uses a left handed coordinate system)
		// Since our 2D coordinate system is in the XZ plane (X = Horiz, Z = Vert), we want the sign of the Y value of the cross product.
		// If the Y value is POSITIVE, then the plane projection is in the +Y (and therefore a POSITIVE rotation).
		// If the Y value is NEGATIVE, then the plane projection is in the -Y (and therefore a NEGATIVE rotation).

		// Positive Rotation is COUNTERCLOCKWISE (to the left)
		// Negative Rotation is CLOCKWISE (to the right)
		float AngleDirectionMultipler = Cross.Y >= 0.0f ? -1.0f : 1.0f; // Note: This is backwards from what I was expecting, whereas the 2D version yields what I was expecting
		AngleDegrees *= AngleDirectionMultipler;

		//UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("-----------------------------"));
		//UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("Cross:                   %s"), *Cross.ToString());
		//UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("AngleDirectionMultipler: %0.2f"), AngleDirectionMultipler);
		//UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("AngleDegrees:            %0.2f"), AngleDegrees);

		FRotator NewPlayerShipRotation = UKismetMathLibrary::MakeRotator(0.0f, AngleDegrees, 0.0f);
		SetActorRotation(NewPlayerShipRotation);

		// NOTE:
		// I could also get the angle using Atan2, but there are two caveats:
		// (1) I need to pass X as the first param and Y as the second, which is backwards from what I would expect
		// (2) I need to flip the result (multiply by -1), which is also not what I would expect
		// It would be called like this:
		// float AtanDegrees = -1.0f * FMath::RadiansToDegrees(FMath::Atan2(MovementDirection.X, MovementDirection.Y));
		// float AtanAngleDegrees = -1.0f * FMath::RadiansToDegrees(FMath::Atan2(MovementDirection.X, MovementDirection.Y));
		// FRotator NewPlayerShipRotation = UKismetMathLibrary::MakeRotator(0.0f, AtanAngleDegrees, 0.0f);
		// SetActorRotation(NewPlayerShipRotation);
		// UE_LOG(LogPlayerShipPawnMovement, Log, TEXT("AtanAngleDegrees: %0.3f"), AtanAngleDegrees);

		// Activate the ship exhaust particle
		if (ShipExhaustParticleComp != nullptr)
		{
			ShipExhaustParticleComp->Activate();
		}
	}
	else
	{
		// Deactivate the ship exhaust particle
		if (ShipExhaustParticleComp != nullptr)
		{
			ShipExhaustParticleComp->Deactivate();
		}
	}
}

void APlayerShipPawn::UpdateGamepadAimFiring()
{
	// If the aiming direction is greater than 0, then the player is aiming/firing with the gamepad right thumbstick
	if (AimingDirection.SquaredLength() > 0.0f)
	{
		// Fire a projectile if enough time has elapsed from the last time a projectile was fired
		if (TimeSinceLastShot >= FireRate)
		{
			if (RightStickDebugBehavior == ERightStickDebugBehavior::FireOnly || RightStickDebugBehavior == ERightStickDebugBehavior::FireAndLineTrace)
			{
				// Get the aiming direction in 3D
				//UE_LOG(LogTemp, Warning, TEXT("-----------------------"));
				//UE_LOG(LogTemp, Warning, TEXT("AimingDirection: %s"), *AimingDirection.ToString());
				FVector AimingDirection3D = FVector(AimingDirection.X, 0.0f, AimingDirection.Y);

				// Get the angle between the world up vector (+Z axis) and the movement input direction
				float Dot = FVector::UnitZ().Dot(AimingDirection3D);
				float Acos = FMath::Acos(Dot);
				float AngleDegrees = FMath::RadiansToDegrees(Acos);

				FVector Cross = FVector::CrossProduct(FVector::UnitZ(), AimingDirection3D);
				float AngleDirectionMultipler = Cross.Y >= 0.0f ? -1.0f : 1.0f;
				AngleDegrees *= AngleDirectionMultipler;

				FRotator AimingRotation = UKismetMathLibrary::MakeRotator(0.0f, AngleDegrees, 0.0f);
				//UE_LOG(LogTemp, Warning, TEXT("AimingRotation: %s"), *AimingRotation.ToString());

				// Update the SatelliteWeapon aiming
				UpdateSatelliteWeaponAimRotation(AimingRotation);

				// Fire projectiles
				FireProjectile(AimingRotation);

				// If the player is using the gamepad right-thumbstick, then hide the mouse cursor
				if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
				{
					SetMouseCursorVisiblityFromInput(PlayerController, false);
				}
			}
		}

		// ==========================================================================
		// Check player collision with border
		if (RightStickDebugBehavior == ERightStickDebugBehavior::LineTraceOnly || RightStickDebugBehavior == ERightStickDebugBehavior::FireAndLineTrace)
		{
			if (UWorld* World = GetWorld())
			{
				const float TraceLineLength = 1000.0f;
				const FVector& TraceStartPos = GetActorLocation();
				const FVector& TraceEndPos = GetActorLocation() + FVector(AimingDirection.X, 0.0f, AimingDirection.Y) * TraceLineLength;

				// Draw debug line for visual feedback
				DrawDebugLine(
					World,
					TraceStartPos,
					TraceEndPos,
					FColor::Cyan);

				TArray<FHitResult> HitResults;
				FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::InitType::AllStaticObjects);

				bool bLineTraceSuccess =
					World->LineTraceMultiByObjectType(
						HitResults,
						TraceStartPos,
						TraceEndPos,
						ObjectQueryParams);

				if (bLineTraceSuccess)
				{
					if (HitResults.Num() > 0)
					{
						const FHitResult& HitResult = HitResults[0];
						UE_LOG(LogTemp, Warning, TEXT("----------------------------"));
						UE_LOG(LogTemp, Warning, TEXT("HitResult.ImpactPoint:      %s"), *HitResult.ImpactPoint.ToString());
						UE_LOG(LogTemp, Warning, TEXT("HitResult.ImpactNormal:     %s"), *HitResult.ImpactNormal.ToString());
						UE_LOG(LogTemp, Warning, TEXT("HitResult.Normal:           %s"), *HitResult.Normal.ToString());
						UE_LOG(LogTemp, Warning, TEXT("HitResult.Distance:         %f"), HitResult.Distance);
						UE_LOG(LogTemp, Warning, TEXT("HitResult.PenetrationDepth: %f"), HitResult.PenetrationDepth);
						UE_LOG(LogTemp, Warning, TEXT("HitResult.bBlockingHit:     %d"), HitResult.bBlockingHit);

						// Draw reflection vector
						FVector ReflectionVector = FMath::GetReflectionVector(FVector(AimingDirection.X, 0.0f, AimingDirection.Y), HitResult.ImpactNormal);
						FVector ReflectEndPos = HitResult.ImpactPoint + ReflectionVector * 500.0f;
						DrawDebugLine(
							World,
							HitResult.ImpactPoint,
							ReflectEndPos,
							FColor::Green);
					}
				}
			}
		}
	}
}

void APlayerShipPawn::UpdateExhaust()
{
	if (ShipExhaustFlipbookComp != nullptr)
	{
		if (MovementDirection.SquaredLength() > 0.0f || bIsDashing)
		{
			if (ShipExhaustFlipbookComp->bHiddenInGame)
			{
				ShipExhaustFlipbookComp->SetHiddenInGame(false);
			}
		}
		else
		{
			if (!ShipExhaustFlipbookComp->bHiddenInGame)
			{
				ShipExhaustFlipbookComp->SetHiddenInGame(true);
			}
		}
	}
}

void APlayerShipPawn::DisablePlayer()
{
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);
}

void APlayerShipPawn::EnablePlayer()
{
	// Hides visible components
	SetActorHiddenInGame(false);

	// Disables collision components
	SetActorEnableCollision(true);

	// Stops the Actor from ticking
	SetActorTickEnabled(true);

	// Reset powerup time
	TotalMultipliersCollected = 0;
	NumMultipliersCollectedForPowerup = 0;
	OnPlayerPowerupTimerUpdated.Broadcast(0.0f);
	ResetPowerupLevel();

	// Reset the Dash values
	bIsDashing = false;
	//DashRechargeTimeElapsed = 0.0f;
	DashRechargeTimeElapsed = DashRechargeTime; // Start player with full dash ability
	DashTimeElapsed = 0.0f;
	bDashReadyAnimPlayed = true; // Don't play the dash ready animation when the player first starts
}

void APlayerShipPawn::KillPlayerFromSelfDestruct()
{
	KillPlayer();
}

FVector APlayerShipPawn::GetEnemySpawnSourcePosition() const
{
	// If the player is moving, get an offset position in front of the player's ship.
	// This prevents an "issue" where the enemies mostly seems to spawn "next to" or behind the player (because of the spawn delay)
	FVector Offset = FVector::ZeroVector;
	if (MovementDirection.SquaredLength() > 0.0f || bIsDashing)
	{
		Offset = GetActorUpVector() * EnemySpawnPosOffsetDistance;
	}
	return GetActorLocation() + Offset;
}

void APlayerShipPawn::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString OtherActorName = OtherActor != nullptr ? OtherActor->GetName() : "(invalid)";
	UE_LOG(LogPlayerShipPawn, Verbose, TEXT("APlayerShipPawn::OnCollisionOverlap - OtherActor: %s"), *OtherActorName);

	if (AEnemyBase* OverlappedEnemy = Cast<AEnemyBase>(OtherActor))
	{
		// The player has collided with an enemy
		if (bIsDashing)
		{
			// Player is Dashing. Kill the enemy.
			OverlappedEnemy->DestroyEnemy(true);
		}
		else
		{
			// Player is not Dashing. Kill the player.
			KillPlayer();
		}
	}
}

void APlayerShipPawn::OnGameStarted()
{
	// TODO: Call enabled function here, but first ensure NOT dead
	bPlayerDead = false;
}

void APlayerShipPawn::OnGameEnded(
	int32 FinalScore,
	int32 SelectedShipSpriteIndex,
	int32 NumEnemiesDefeated,
	int32 NumScoreMultipliersCollected,
	int32 NumEnemiesDefeatedWithBoost,
	int32 NumProjectilesFired,
	int32 CurrentScoreMultiplier)
{
	// The game has ended. Force show the mouse cursor, as it likely was hidden from right-stick aiming
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		SetMouseCursorVisiblityFromInput(PlayerController, true);
	}
}

void APlayerShipPawn::OnPlayerShipSelected(int32 ShipSpriteIndex)
{
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (UPaperSprite* SelectedShipSprite = GameInstance->GetShipSpriteForIndex(ShipSpriteIndex))
		{
			// Set the player ship sprite
			if (PaperSpriteComp != nullptr)
			{
				PaperSpriteComp->SetSprite(SelectedShipSprite);
			}
				
			// Set the satellite weapon sprites
			if (SatelliteWeaponSprite1)
			{
				SatelliteWeaponSprite1->SetSprite(SelectedShipSprite);
			}

			if (SatelliteWeaponSprite2)
			{
				SatelliteWeaponSprite2->SetSprite(SelectedShipSprite);
			}

			if (SatelliteWeaponSprite3)
			{
				SatelliteWeaponSprite3->SetSprite(SelectedShipSprite);
			}

			if (SatelliteWeaponSprite4)
			{
				SatelliteWeaponSprite4->SetSprite(SelectedShipSprite);
			}
		}
	}
}

void APlayerShipPawn::KeyboardMoveTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D MoveTriggeredInput = InputActionValue.Get<FVector2D>();
	MovementDirection = MoveTriggeredInput.GetSafeNormal();
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::KeyboardMoveTriggered - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::KeyboardMoveCompleted(const FInputActionValue& InputActionValue)
{
	MovementDirection = FVector2D::ZeroVector;
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::KeyboardMoveCompleted - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::GamepadMoveTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D MoveTriggeredInput = InputActionValue.Get<FVector2D>();
	MovementDirection = MoveTriggeredInput.GetSafeNormal();
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::GamepadMoveTriggered - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::GamepadMoveCompleted(const FInputActionValue& InputActionValue)
{
	MovementDirection = FVector2D::ZeroVector;
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::GamepadMoveCompleted - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::GamepadAimTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D AimTriggeredInput = InputActionValue.Get<FVector2D>();
	AimingDirection = AimTriggeredInput.GetSafeNormal();
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::GamepadAimTriggered - %s"), *AimingDirection.ToString());
}

void APlayerShipPawn::GamepadAimCompleted(const FInputActionValue& InputActionValue)
{
	AimingDirection = FVector2D::ZeroVector;
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::GamepadAimCompleted - AimingDirection: %s"), *AimingDirection.ToString());
}

void APlayerShipPawn::Fire(const FInputActionValue& InputActionValue)
{
	bool FireInput = InputActionValue.Get<bool>();
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::Fire - FireInput: %d"), FireInput);

	// Fire a projectile if enough time has elapsed from the last time a projectile was fired
	if (TimeSinceLastShot >= FireRate)
	{
		FRotator PlayerShipRotation = GetActorRotation();
		FireProjectile(PlayerShipRotation);

		// If the player is using gamepad fire, then hide the mouse cursor
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			SetMouseCursorVisiblityFromInput(PlayerController, false);
		}
	}
}

void APlayerShipPawn::MouseFire(const FInputActionValue& InputActionValue)
{
	// Quick fix: dont let the player fire outside of the game
	if (IsPlayerDisabled())
	{
		return;
	}

	bool MouseFireInput = InputActionValue.Get<bool>();
	UE_LOG(LogPlayerShipPawnInput, Verbose, TEXT("APlayerShipPawn::MouseFire - FireInput: %d"), MouseFireInput);

	if (TimeSinceLastShot < FireRate)
	{
		// Exit if not enough time has elapsed since the last time a projectile was fired
		return;
	}

	// Get the player controller in order to get the world mouse position
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// If the player is pressing MouseFire, then ensure the mouse cursor is shown
		SetMouseCursorVisiblityFromInput(PlayerController, true);

		// Get the player ship's current position
		FVector PlayerShipPosition = GetActorLocation();

		// Get the mouse position in world coordinates in the XZ plane
		FVector MouseWorldPosition, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldPosition, MouseWorldDirection);
		MouseWorldPosition = FVector(MouseWorldPosition.X, 0.0f, MouseWorldPosition.Z); // "Flatten" the mouse position onto the XZ plane

		// Note: This didn't work for some reason. Rotation seemed off by 90 degrees. Using my standard Dot and Cross calculations instead.
		// Create a rotation representing the vector from the player position to the mouse position
		// FRotator MouseAimingRotation = UKismetMathLibrary::FindLookAtRotation(PlayerShipPosition, MouseWorldPosition);
	
		// Use mouse hit for more accurate/consistent mouse targeting. This is needed in order to use a Perspective Projection camera.
		FVector MouseHitPosition;
		FHitResult CursorHitResultByChannel;
		bool bMouseClickHItResult = PlayerController->GetHitResultUnderCursorByChannel(
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), // MouseBlockerTrace
			true, // bTraceComplex -- set to false
			CursorHitResultByChannel);
		if (bMouseClickHItResult)
		{
			// Get the mouse hit position of the line trace onto the mouse blocking object
			//MouseHitPosition = CursorHitResultByChannel.ImpactPoint;
			MouseWorldPosition = CursorHitResultByChannel.ImpactPoint; // Set the position in case the mouse trace hit fails

			// Draw debug hit
			if (bShowMouseHitDebug)
			{
				AActor* HitActor = CursorHitResultByChannel.GetActor();
				FString HitActorName = HitActor != nullptr ? HitActor->GetName() : "(invalid HitActor)";
				//UE_LOG(LogTemp, Warning, TEXT("Channel HitActorName: %s"), *HitActorName);
				DrawDebugSphere(GetWorld(),
					CursorHitResultByChannel.ImpactPoint, // Can also use CursorHitResultByChannel.Location
					10.0f, // radius
					10, // segments
					FColor::Emerald,
					true, // persistent
					100.0f, // lifetime
					ESceneDepthPriorityGroup::SDPG_World,
					5.0f); // thickness
			}
		}

		// Get the direction from the player ship position to the mouse world position
		FVector MouseAimingDirection = (MouseWorldPosition - PlayerShipPosition).GetSafeNormal();

		// Get the angle between the world up vector (+Z axis) and the mouse aiming direction
		float Dot = FVector::UnitZ().Dot(MouseAimingDirection);
		float Acos = FMath::Acos(Dot);
		float AngleDegrees = FMath::RadiansToDegrees(Acos);

		FVector Cross = FVector::CrossProduct(FVector::UnitZ(), MouseAimingDirection);
		float AngleDirectionMultipler = Cross.Y >= 0.0f ? -1.0f : 1.0f;
		AngleDegrees *= AngleDirectionMultipler;

		// Create the rotation from the aiming angle
		FRotator MouseAimingRotation = UKismetMathLibrary::MakeRotator(0.0f, AngleDegrees, 0.0f);

		// Update the SatelliteWeapon aiming
		UpdateSatelliteWeaponAimRotation(MouseAimingRotation);

		// Fire the projectile using the aiming rotation
		FireProjectile(MouseAimingRotation);
	}
}

void APlayerShipPawn::InputDash(const FInputActionValue& InputActionValue)
{
	// Do not allow dashing if the player is dead or "disabled"
	if (bPlayerDead || IsPlayerDisabled())
	{
		return;
	}

	if (bIsDashing)
	{
		// Player is already dashing. Ignore this dash input.
		return;
	}

	// Do not allow the player to dash while the dash meter is recharging
	if (DashRechargeTimeElapsed < DashRechargeTime)
	{
		return;
	}

	bIsDashing = true;
	DashRechargeTimeElapsed = 0.0f;
	DashTimeElapsed = 0.0f;
	ShowDashShield();

	// Enlarge the collision sphere radius
	if (SphereComp != nullptr)
	{
		PreDashCollisionSphereRadius = SphereComp->GetUnscaledSphereRadius();
		SphereComp->SetSphereRadius(DashCollisionSphereRadius, true);
	}

	// Play boost sound
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::ShipBoostSound);
	}

	// Enable dash exhaust particle
	if (DashExhaustParticleComp != nullptr)
	{
		DashExhaustParticleComp->Activate();
	}

	OnPlayerDashUpdated.Broadcast(0.0f);
}

void APlayerShipPawn::InputPause(const FInputActionValue& InputActionValue)
{
	// Do not allow pausing if the player is dead or "disabled"
	if (bPlayerDead || IsPlayerDisabled())
	{
		return;
	}

	// Handle Pause/Unpause
	bool bGameIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	if (bGameIsPaused)
	{
		// Game is Paused. Unpause the game.
		ASpaceShooterGameState::OnRequestUnpauseGame.Broadcast();
	}
	else
	{
		// Game is not Paused. Pause the game.
		ASpaceShooterGameState::OnRequestPauseGame.Broadcast();

		// Ensure the mouse cursor does not auto-hide while the pause screen is open
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			SetMouseCursorVisiblityFromInput(PlayerController, true);
		}
	}
}

void APlayerShipPawn::InputCleanPause(const FInputActionValue& InputActionValue)
{	
	// Do not allow clean pause if player is dead or disabled (meaning dead or in menus)
	if (bPlayerDead || IsPlayerDisabled())
	{
		return;
	}

	UWorld* World = GetWorld();
	bool bIsGamePaused = UGameplayStatics::IsGamePaused(World);
	UE_LOG(LogPlayerShipPawn, Warning, TEXT("Clean Pausing/Unpausing game: %d"), bIsGamePaused);
	UGameplayStatics::SetGamePaused(World, !bIsGamePaused);
}

void APlayerShipPawn::FireProjectile(FRotator ProjectileRotation)
{
	// Do not allow projectile firing while dashing
	if (bIsDashing)
	{
		return;
	}

	if (ensureMsgf(
		ProjectileClass != nullptr,
		TEXT("%s - ProjectileClass not set. Set it in the PlayerShip blueprint."), ANSI_TO_TCHAR(__FUNCTION__)))
	{
		if (UWorld* World = GetWorld())
		{
			// TODO: Use Firepoint
			FVector PlayerShipPosition = GetActorLocation();
			//FRotator PlayerShipRotation = GetActorRotation();

			FirePointRotatorComp->SetWorldRotation(ProjectileRotation);

			FActorSpawnParameters ProjectileSpawnParameters;
			ProjectileSpawnParameters.Name = TEXT("Projectile_");
			ProjectileSpawnParameters.Instigator = this;
			ProjectileSpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			ProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			if (FirePointComp1 != nullptr)
			{
				if (SpaceShooterGameState != nullptr)
				{
					SpaceShooterGameState->FireProjectile(FirePointComp1->GetComponentLocation(), FirePointComp1->GetComponentRotation(), this);
				}
			}

			if (FirePointComp2 != nullptr)
			{
				if (SpaceShooterGameState != nullptr)
				{
					SpaceShooterGameState->FireProjectile(FirePointComp2->GetComponentLocation(), FirePointComp2->GetComponentRotation(), this);
				}
			}

			// Play the shoot sound
			PlayShootSound();

			// Reset time since last shot
			TimeSinceLastShot = 0.0f;

			// Fire projectiles from Satellite weapons
			FireProjectileFromSatelliteWeapon(World, ProjectileSpawnParameters);
		}
	}
}

void APlayerShipPawn::PlayShootSound()
{
	if (bShootSoundEnabled)
	{
		if (bPlayShootSoundDuringPowerup && PlayerHasPowerup())
		{
			if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				GameInstance->PlaySound(ESoundEffect::ShipShootSound);
			}
		}
	}
}

void APlayerShipPawn::KillPlayer()
{
	if (bPlayerInvincible)
	{
		return;
	}

	// Play death sound
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::ShipExplosionSound);
	}

	// Play explosion effect
	if (PlayerExplosionEffect != nullptr && PlayerExplosionEffect->IsValid())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PlayerExplosionEffect.Get(), GetActorLocation());
	}

	// Destroy player ship
	//Destroy();

	// Disable the player ship instead of destroying it (so the camera doesn't look in the wrong place)
	DisablePlayer();

	// Mark player as dead. TODO: Add delegate and notify.
	bPlayerDead = true;

	// TEMP: Stop enemy spawning manually
	AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
	if (EnemySpawner != nullptr)
	{
		// Disable enemy spawning
		EnemySpawner->SetSpawningEnabled(false);
	}

	// Disable powerups
	RemoveSatelliteWeapon();

	OnPlayerShipDestroyed.Broadcast();
}

void APlayerShipPawn::SetMouseCursorVisiblityFromInput(APlayerController* const PlayerController, bool bCursorVisible)
{
	if (PlayerController != nullptr)
	{
		PlayerController->SetShowMouseCursor(bCursorVisible);
	}

	// Force the SlateApplication to refresh the mouse cursor, otherwise it will not change until there is some mouse movement
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	SlateApplication.SetAllUserFocusToGameViewport(); // TODO: Is this necessary?
	SlateApplication.QueryCursor();

	// TEST: Get the GameViewportClient and check the settings:
	// UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport();

	// May need to change these settings to keep the mouse cursor on screen
	// FInputModeGameAndUI InputMode;
	// InputMode.SetHideCursorDuringCapture(false);
	// PlayerController->SetInputMode(InputMode);
}

void APlayerShipPawn::UpdateSatelliteWeaponRotation(float DeltaTime)
{
	if (SatelliteWeaponRotatorComp != nullptr)
	{
		// Multiply the rotation angle by 1 or -1 depending on direction
		//float RotationDirMultiple = RotationDirection == RotationDirection.Clockwise ? -1.0f : 1.0f;

		// Increment the rotation angle in the desired rotation direction
		CurrentSatelliteWeaponRotationAngle += SatelliteWeaponRotationSpeed * DeltaTime /* *RotationDirMultiple */;

		// Ensure the rotation angle stays within (-360, 360)
		if (CurrentSatelliteWeaponRotationAngle >= 360.0f)
		{
			CurrentSatelliteWeaponRotationAngle = 0.0f;
		}
		else if (CurrentSatelliteWeaponRotationAngle <= -360.0f)
		{
			CurrentSatelliteWeaponRotationAngle = 0.0f;
		}

		// Set the new satellite weapon world rotation 
		FRotator NewSatelliteWeaponRotation = UKismetMathLibrary::MakeRotator(0.0f, CurrentSatelliteWeaponRotationAngle, 0.0f);
		SatelliteWeaponRotatorComp->SetWorldRotation(NewSatelliteWeaponRotation);
	}
}

void APlayerShipPawn::UpdateSatelliteWeaponAimRotation(FRotator AimRotation)
{
	UpdateSatelliteWeaponAimRotation(AimRotation, SatelliteWeaponSprite1);
	UpdateSatelliteWeaponAimRotation(AimRotation, SatelliteWeaponSprite2);
	UpdateSatelliteWeaponAimRotation(AimRotation, SatelliteWeaponSprite3);
	UpdateSatelliteWeaponAimRotation(AimRotation, SatelliteWeaponSprite4);
}

void APlayerShipPawn::UpdateSatelliteWeaponAimRotation(FRotator AimRotation, UPaperSpriteComponent* const SatelliteWeapon)
{
	if (SatelliteWeapon != nullptr)
	{
		SatelliteWeapon->SetWorldRotation(AimRotation);
	}
}

void APlayerShipPawn::FireProjectileFromSatelliteWeapon(UWorld* const World, const FActorSpawnParameters& ProjectileSpawnParameters)
{
	FireProjectileFromSatelliteWeapon(World, SatelliteWeaponSprite1, ProjectileSpawnParameters);
	FireProjectileFromSatelliteWeapon(World, SatelliteWeaponSprite2, ProjectileSpawnParameters);
	FireProjectileFromSatelliteWeapon(World, SatelliteWeaponSprite3, ProjectileSpawnParameters);
	FireProjectileFromSatelliteWeapon(World, SatelliteWeaponSprite4, ProjectileSpawnParameters);
}

void APlayerShipPawn::FireProjectileFromSatelliteWeapon(
	UWorld* const World,
	UPaperSpriteComponent* const SatelliteWeapon,
	const FActorSpawnParameters& ProjectileSpawnParameters)
{
	if (SatelliteWeapon == nullptr)
	{
		return;
	}

	// Do not fire a projectile if this satellite weapon is disabled
	if (!SatelliteWeapon->GetVisibleFlag())
	{
		return;
	}

	if (SpaceShooterGameState != nullptr)
	{
		SpaceShooterGameState->FireProjectile(SatelliteWeapon->GetComponentLocation(), SatelliteWeapon->GetComponentRotation(), this);
	}
}

void APlayerShipPawn::DisableSatelliteWeapons()
{
	DisableSatelliteWeapon(SatelliteWeaponSprite1);
	DisableSatelliteWeapon(SatelliteWeaponSprite2);
	DisableSatelliteWeapon(SatelliteWeaponSprite3);
	DisableSatelliteWeapon(SatelliteWeaponSprite4);
}

void APlayerShipPawn::DisableSatelliteWeapon(UPaperSpriteComponent* const SatelliteWeapon)
{
	if (SatelliteWeapon != nullptr)
	{
		SatelliteWeapon->SetHiddenInGame(true, true);
		SatelliteWeapon->SetVisibility(false, true);
		SatelliteWeapon->SetComponentTickEnabled(false);
	}
}

void APlayerShipPawn::EnableSatelliteWeapon(UPaperSpriteComponent* const SatelliteWeapon)
{
	if (SatelliteWeapon != nullptr)
	{
		SatelliteWeapon->SetHiddenInGame(false, false);
		SatelliteWeapon->SetVisibility(true, true);
		SatelliteWeapon->SetComponentTickEnabled(true);
	}
}

void APlayerShipPawn::AddSatelliteWeapon()
{
	//EnableSatelliteWeapon(SatelliteWeaponSprite1);
	//EnableSatelliteWeapon(SatelliteWeaponSprite2);
	PowerupActiveTimer = PowerupActiveTime;

	// Play Powerup Earned sound
	if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->PlaySound(ESoundEffect::PowerupEarnedSound);
	}

	// Play "Power Up" particle effect
	// Note: The particle system asset has "Local Space" enabled, which allows the particles to "follow" the emitter.
	// TODO: Attach to a component with absolute rotation so the particles dont rotate with the player ship
	if (PlayerPowerupEffectLarge != nullptr)
	{
		//UNiagaraFunctionLibrary::SpawnSystemAttached(PlayerPowerupEffectLarge, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
		UNiagaraFunctionLibrary::SpawnSystemAttached(PlayerPowerupEffectLarge, PowerupEffectAttachPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	IncrementPowerupLevel();
}

void APlayerShipPawn::RemoveSatelliteWeapon()
{
	DisableSatelliteWeapon(SatelliteWeaponSprite1);
	DisableSatelliteWeapon(SatelliteWeaponSprite2);
	DisableSatelliteWeapon(SatelliteWeaponSprite3);
	DisableSatelliteWeapon(SatelliteWeaponSprite4);
	PowerupActiveTimer = 0.0f;
}

bool APlayerShipPawn::PlayerHasPowerup() const
{
	bool bPlayerHasPowerup = false;
	if (SatelliteWeaponSprite1 != nullptr)
	{
		// If the satellite weapon is visible, then it is active (and therefore the player has a powerup)
		bPlayerHasPowerup = SatelliteWeaponSprite1->GetVisibleFlag();
	}
	return bPlayerHasPowerup;
}

void APlayerShipPawn::PowerupTimerElapsed()
{
	RemoveSatelliteWeapon();
	ResetPowerupLevel();
}

void APlayerShipPawn::OnScoreMultiplierPickedUp(int32 ScoreMultiplierValue)
{
	// TODO: Handle this stuff: PickupItemPercentChanged

	// Get game state
	if (UWorld* World = GetWorld())
	{
		if (ASpaceShooterGameState* GameState = Cast<ASpaceShooterGameState>(World->GetGameState()))
		{
			GameState->AddCurrentScoreMultiplier(ScoreMultiplierValue);
		}
	}

	// Increment values and notify
	//CurrentScoreMultiplier += ScoreMultiplierValue;
	//OnPlayerMultiplierChanged.Broadcast(CurrentScoreMultiplier);

	TotalMultipliersCollected++;

	// Do not increment the num powerups collected if the player has a powerup
	if (!PlayerHasPowerup())
	{
		NumMultipliersCollectedForPowerup++;
	}

	float Percent = (float)NumMultipliersCollectedForPowerup / (float)NumMultipliersNeededForPowerup;
	//OnPickupItemPercentChanged.Broadcast(Percent);
	ASpaceShooterGameState::OnPickupItemPercentChanged.Broadcast(Percent);
	PickupItemPercentChanged(Percent);

	if (NumMultipliersCollectedForPowerup >= NumMultipliersNeededForPowerup)
	{
		NumMultipliersCollectedForPowerup = 0;
	}
}

void APlayerShipPawn::PickupItemPercentChanged(float Percent)
{
	// Check the pickup item percent
	if ((int32)Percent == 1)
	{
		// Pickup count at 100%. Add a powerup weapon (this will also start the "drain").
		AddSatelliteWeapon();
	}
	else
	{
		// Total is not at 100%. Notify listeners pickup percent has changed (only if player does not have powerup)
		if (!PlayerHasPowerup())
		{
			OnPlayerPowerupTimerUpdated.Broadcast(Percent);
		}
		else
		{
			// Player has an active powerup, so the meter is draining. Add extra time to the meter.
			PowerupActiveTimer += PowerupPickupAddTime;

			// Play powerup time add sound
			if (USpaceShooterGameInstance* GameInstance = Cast<USpaceShooterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				if (CurrentPowerupLevel < MaxPowerupLevels)
				{
					// Not max powerup level
					GameInstance->PlaySound(ESoundEffect::PowerupLevelUpSound);
				}
				else
				{
					// Max powerup level
					//GameInstance->PlaySound(ESoundEffect::PowerupTimeAddedSound); // RKS: I dislike how this sounds. I prefer the standard powerup levelup sound.
					GameInstance->PlaySound(ESoundEffect::PowerupLevelUpSound);
				}
			}

			// Play powerup time added effect
			if (PlayerPowerupEffectSmall != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(PlayerPowerupEffectSmall, PowerupEffectAttachPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
			}

			IncrementPowerupLevel();
		}
	}
}

void APlayerShipPawn::ShowDashShield()
{
	// Show dash shield sprite
	if (DashShieldSpriteComp != nullptr)
	{
		DashShieldSpriteComp->SetHiddenInGame(false, false);
		DashShieldSpriteComp->SetVisibility(true, true);
		//DashShieldSpriteComp->SetComponentTickEnabled(true);
	}
}

void APlayerShipPawn::HideDashShield()
{
	// Hide dash shield sprite
	if (DashShieldSpriteComp != nullptr)
	{
		DashShieldSpriteComp->SetHiddenInGame(true, true);
		DashShieldSpriteComp->SetVisibility(false, true);
		//DashShieldSpriteComp->SetComponentTickEnabled(false);
	}
}

void APlayerShipPawn::OnDashReadyAnimationFinished()
{
	if (DashReadyFlipbookComp != nullptr)
	{
		DashReadyFlipbookComp->SetHiddenInGame(true);
	}
}

void APlayerShipPawn::ResetPowerupLevel()
{
	CurrentPowerupLevel = 0;
}

void APlayerShipPawn::IncrementPowerupLevel()
{
	switch (CurrentPowerupLevel)
	{
		case 0:
			EnableSatelliteWeapon(SatelliteWeaponSprite1);
			CurrentPowerupLevel++;
			break;

		case 1:
			EnableSatelliteWeapon(SatelliteWeaponSprite2);
			CurrentPowerupLevel++;
			break;

		case 2:
			EnableSatelliteWeapon(SatelliteWeaponSprite3);
			CurrentPowerupLevel++;
			break;

		case 3:
			EnableSatelliteWeapon(SatelliteWeaponSprite4);
			CurrentPowerupLevel++;
			break;
	}
}

