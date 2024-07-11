// Copyright 2024 Richard Skala

#include "PlayerShipPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
}

// Called when the game starts or when spawned
void APlayerShipPawn::BeginPlay()
{
	UE_LOG(LogPlayerShipPawn, Log, TEXT("APlayerShipPawn::BeginPlay - %s"), *GetName());
	Super::BeginPlay();
}

