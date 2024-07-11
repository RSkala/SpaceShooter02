// Copyright 2024 Richard Skala

#include "PlayerShipPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "PaperSpriteComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawn, Log, All)

// Sets default values
APlayerShipPawn::APlayerShipPawn()
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::APlayerShipPawn - %s"), *GetName());

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

// Called every frame
void APlayerShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Update player ship from movement and direction inputs
	//UE_LOG(LogPlayerShipPawn, Log, TEXT("MovementDirection: %s"), *MovementDirection.ToString());
	//UE_LOG(LogPlayerShipPawn, Log, TEXT("AimingDirection:   %s"), *AimingDirection.ToString());
}

// Called to bind functionality to input
void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::SetupPlayerInputComponent - %s"), *GetName());
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
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::BeginPlay - %s"), *GetName());
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

void APlayerShipPawn::KeyboardMoveTriggered(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::KeyboardMoveTriggered - %s"), *GetName());
	FVector2D MoveTriggeredInput = InputActionValue.Get<FVector2D>();
	MovementDirection = MoveTriggeredInput.GetSafeNormal();
}

void APlayerShipPawn::KeyboardMoveCompleted(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::KeyboardMoveCompleted - %s"), *GetName());
	MovementDirection = FVector2D::ZeroVector;
}

void APlayerShipPawn::GamepadMoveTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D MoveTriggeredInput = InputActionValue.Get<FVector2D>();
	MovementDirection = MoveTriggeredInput.GetSafeNormal();

	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::GamepadMoveTriggered - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::GamepadMoveCompleted(const FInputActionValue& InputActionValue)
{
	MovementDirection = FVector2D::ZeroVector;
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::GamepadMoveCompleted - MovementDirection: %s"), *MovementDirection.ToString());
}

void APlayerShipPawn::GamepadAimTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D AimTriggeredInput = InputActionValue.Get<FVector2D>();
	AimingDirection = AimTriggeredInput.GetSafeNormal();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::GamepadAimTriggered - %s"), *AimingDirection.ToString());
}

void APlayerShipPawn::GamepadAimCompleted(const FInputActionValue& InputActionValue)
{
	AimingDirection = FVector2D::ZeroVector;
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::GamepadAimCompleted - AimingDirection: %s"), *AimingDirection.ToString());
}

void APlayerShipPawn::Fire(const FInputActionValue& InputActionValue)
{
	bool FireInput = InputActionValue.Get<bool>();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::Fire - FireInput: %d"), FireInput);
}

