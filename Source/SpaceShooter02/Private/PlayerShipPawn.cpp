// Copyright 2024 Richard Skala

#include "PlayerShipPawn.h"

#include "Camera/CameraComponent.h"
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

#include "EnemyBase.h"
#include "EnemySpawner.h"
#include "ProjectileBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawn, Warning, All)
DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawnInput, Warning, All)
DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawnMovement, Log, All)

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

	FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
	FirePointComp->SetupAttachment(RootComponent);

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
}

void APlayerShipPawn::UpdateMovement(float DeltaTime)
{
	UE_LOG(LogPlayerShipPawnMovement, Verbose, TEXT("APlayerShipPawn::UpdateMovement: %s"), *MovementDirection.ToString());

	if (MovementDirection.SquaredLength() > 0.0f)
	{
		// Valid movement input - update the player ship position and rotation
		
		// --------------------------------------------------------
		// Update the ship position
		
		// Get player ship's current position
		FVector PlayerShipPosition = GetActorLocation();

		// Get the distance to move this frame using the movement direction
		// Movement formula, Distance = Rate * Time
		FVector2D MovementAmount2D = MovementDirection * MoveSpeed * DeltaTime;

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
			float CollisionDistance = MoveSpeed * DeltaTime * CollisionLineTraceOffset;

			// Check collision against walls in the X movement direction
			TArray<FHitResult> HitResultsXDir;
			const FVector& TraceEndPosXDir = TraceStartPos + FVector(MovementDirection.X, 0.0f, 0.0f).GetSafeNormal() * CollisionDistance;
			if (World->LineTraceMultiByObjectType(HitResultsXDir, TraceStartPos, TraceEndPosXDir, ObjectQueryParams))
			{
				// Successful line trace hit in the X direction. Zero out the X movement amount.
				MovementAmount.X = 0.0f;
			}
			
			// Check collision against walls in the Y movement direction (Z world direction)
			TArray<FHitResult> HitResultsYDir;
			const FVector& TraceEndPosYDir = TraceStartPos + FVector(0.0f, 0.0f, MovementDirection.Y).GetSafeNormal() * CollisionDistance;
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
		FVector MovementDirection3D = FVector(MovementDirection.X, 0.0f, MovementDirection.Y);

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
		if (MovementDirection.SquaredLength() > 0.0f)
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

void APlayerShipPawn::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString OtherActorName = OtherActor != nullptr ? OtherActor->GetName() : "(invalid)";
	UE_LOG(LogPlayerShipPawn, Verbose, TEXT("APlayerShipPawn::OnCollisionOverlap - OtherActor: %s"), *OtherActorName);

	if (AEnemyBase* OverlappedEnemy = Cast<AEnemyBase>(OtherActor))
	{
		// Player has collided with an enemy. Kill the player.
		KillPlayer();
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

		// Fire the projectile using the aiming rotation
		FireProjectile(MouseAimingRotation);
	}
}

void APlayerShipPawn::FireProjectile(FRotator ProjectileRotation)
{
	if (ensureMsgf(
		ProjectileClass != nullptr,
		TEXT("%s - ProjectileClass not set. Set it in the PlayerShip blueprint."), ANSI_TO_TCHAR(__FUNCTION__)))
	{
		if (UWorld* World = GetWorld())
		{
			// TODO: Use Firepoint
			FVector PlayerShipPosition = GetActorLocation();
			//FRotator PlayerShipRotation = GetActorRotation();

			FActorSpawnParameters ProjectileSpawnParameters;
			ProjectileSpawnParameters.Name = TEXT("Projectile_");
			ProjectileSpawnParameters.Instigator = this;
			ProjectileSpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			ProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//AProjectileBase* FiredProjectile = World->SpawnActor<AProjectileBase>(ProjectileClass, PlayerShipPosition, PlayerShipRotation, ProjectileSpawnParameters);
			AProjectileBase* FiredProjectile = World->SpawnActor<AProjectileBase>(ProjectileClass, PlayerShipPosition, ProjectileRotation, ProjectileSpawnParameters);
			FiredProjectile->Init(FVector::ZeroVector, FRotator::ZeroRotator); // wip

			// Play the shoot sound
			PlayShootSound();

			// Reset time since last shot
			TimeSinceLastShot = 0.0f;
		}
	}
}

void APlayerShipPawn::PlayShootSound()
{
	if (ensureMsgf(
		PlayerShootSound != nullptr, TEXT("%s - PlayerShootSound not set. Set it in the PlayerShip blueprint."), ANSI_TO_TCHAR(__FUNCTION__)))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerShootSound);
	}
}

void APlayerShipPawn::KillPlayer()
{
	// Play death sound
	if (ensureMsgf(
		PlayerDeathSound != nullptr, TEXT("%s - PlayerDeathSound not set. Set it in the PlayerShip blueprint."), ANSI_TO_TCHAR(__FUNCTION__)))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerDeathSound);
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

