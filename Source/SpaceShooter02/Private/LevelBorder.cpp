// Copyright 2024 Richard Skala

#include "LevelBorder.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

#include "ColorShiftLocalPlayerSubsystem.h"

namespace
{
	static const TCHAR* DefaultLevelBorderSpriteAssetPath = TEXT("/Game/Sprites/Backgrounds/SPR_WhiteSquare");
}

ALevelBorder::ALevelBorder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f)); // Set box extents so it fully encloses the sprite
	BoxComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName); // Set collision to BlockAll
	BoxComp->SetMobility(EComponentMobility::Static); // Set mobility to Static as this should never move

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>("PaperSpriteComp");
	PaperSpriteComp->SetupAttachment(RootComponent);
	PaperSpriteComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); // Disable sprite collision
	PaperSpriteComp->SetMobility(EComponentMobility::Stationary); // Set mobility to Stationary as it will not move (but may be updated)
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Set the default level border sprite (for the CDO only)
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DefaultLevelBorderSpriteFinder(DefaultLevelBorderSpriteAssetPath);
		PaperSpriteComp->SetSprite(DefaultLevelBorderSpriteFinder.Get());
	}
}

void ALevelBorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelBorder::BeginPlay()
{
	Super::BeginPlay();

	// Subscribe to the color shift delegate
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UColorShiftLocalPlayerSubsystem* ColorShiftSubsystem = ULocalPlayer::GetSubsystem<UColorShiftLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			ColorShiftSubsystem->OnColorShift.AddUniqueDynamic(this, &ThisClass::OnColorShift);
		}
	}
}

void ALevelBorder::OnColorShift(FLinearColor LinearColor)
{
	// Update the sprite color
	if (PaperSpriteComp != nullptr)
	{
		PaperSpriteComp->SetSpriteColor(LinearColor);
	}
}

