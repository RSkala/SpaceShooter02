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
}

// Called to bind functionality to input
void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::SetupPlayerInputComponent - %s"), *GetName());
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &ThisClass::MoveTriggered);
		//EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted); // Possibly unnecessary

		// Bind Aiming 
		EnhancedInputComponent->BindAction(InputActionAim, ETriggerEvent::Triggered, this, &ThisClass::AimTriggered);
		//EnhancedInputComponent->BindAction(InputActionAim, ETriggerEvent::Completed, this, &ThisClass::MoveTriggered); // Possibly unnecessary

		// Bind Firing/Shooting 
		EnhancedInputComponent->BindAction(InputActionFire, ETriggerEvent::Started, this, &ThisClass::Fire); // Started: When the player first presses the Fire button
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

void APlayerShipPawn::MoveTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D MoveTriggeredInput = InputActionValue.Get<FVector2D>();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::MoveTriggered - %s, MoveTriggeredInput: %s"), *GetName(), *MoveTriggeredInput.ToString());
}

void APlayerShipPawn::MoveCompleted(const FInputActionValue& InputActionValue)
{
	FVector2D MoveCompletedInput = InputActionValue.Get<FVector2D>();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::MoveCompleted - %s, MoveCompletedInput: %s"), *GetName(), *MoveCompletedInput.ToString());
}

void APlayerShipPawn::AimTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D AimTriggeredInput = InputActionValue.Get<FVector2D>();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::MoveTriggered - %s, AimTriggeredInput: %s"), *GetName(), *AimTriggeredInput.ToString());
}

void APlayerShipPawn::Fire(const FInputActionValue& InputActionValue)
{
	bool FireInput = InputActionValue.Get<bool>();
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::Shoot - %s, FireInput: %d"), *GetName(), FireInput);
}

