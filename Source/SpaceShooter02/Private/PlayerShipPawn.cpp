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
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

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
	SpringArmComp->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0)); // Set the rotation so it is looking down the Y-axis towards the XZ plane (Note: Yaw is Z-axis rotation)

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComp->OrthoWidth = 3840.0f;
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
	}
}

// Called when the game starts or when spawned
void APlayerShipPawn::BeginPlay()
{
	UE_LOG(LogPlayerShipPawn, Verbose, TEXT("APlayerShipPawn::BeginPlay - %s"), *GetName());
	Super::BeginPlay();

	// Setup Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
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
}

void APlayerShipPawn::UpdateMovement(float DeltaTime)
{
	UE_LOG(LogPlayerShipPawnMovement, Verbose, TEXT("APlayerShipPawn::UpdateMovement: %s"), *MovementDirection.ToString());

	if(MovementDirection.Length() > 0.0f)
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
		FVector NewShipPosition = PlayerShipPosition + MovementAmount;
		SetActorLocation(NewShipPosition);

		// ---------------------------------------------------------
		// Update the ship facing direction
		FRotator PlayerShipRotation = GetActorRotation();

		//FVector MovementDirection3D = FVector(MovementDirection.X, PlayerShipPosition.Y, MovementDirection.Y);
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
		float AngleDirectionMultipler = Cross.Y >= 0.0f ? -1.0f : 1.0f;
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
}

