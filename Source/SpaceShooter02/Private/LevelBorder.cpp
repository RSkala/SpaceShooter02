// Copyright 2024 Richard Skala

#include "LevelBorder.h"

#include "Components/BoxComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

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
	UpdateLevelBorderSpriteColor(DeltaTime);
}

void ALevelBorder::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelBorder::UpdateLevelBorderSpriteColor(float DeltaTime)
{
	// Update color hue shift
	ColorCyclingTimer += DeltaTime / SecondsPerCyclingMode;
	FLinearColor LerpedColor(FLinearColor::White);

	switch (ColorCyclingMode)
	{
		case EColorCyclingMode::RedToGreen: LerpedColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, ColorCyclingTimer); break;
		case EColorCyclingMode::GreenToBlue: LerpedColor = FMath::Lerp(FLinearColor::Green, FLinearColor::Blue, ColorCyclingTimer); break;
		case EColorCyclingMode::BlueToRed: LerpedColor = FMath::Lerp(FLinearColor::Blue, FLinearColor::Red, ColorCyclingTimer); break;
		default: break;
	}

	// Update the sprite color
	if (PaperSpriteComp != nullptr)
	{
		PaperSpriteComp->SetSpriteColor(LerpedColor);
	}

	// If the color cycling timer has elapsed, change color cycling modes
	if (ColorCyclingTimer >= 1.0f)
	{
		SwitchToNextColorCyclingMode();
		ColorCyclingTimer = 0.0f;
	}
}

void ALevelBorder::SwitchToNextColorCyclingMode()
{
	uint8 NextColorCyclingMode = ((uint8)ColorCyclingMode + 1) % (uint8)EColorCyclingMode::NumColorCyclingModes;
	ColorCyclingMode = (EColorCyclingMode)NextColorCyclingMode;

	//uint8 NextColorCyclingMode = (uint8)ColorCyclingMode + 1;
	//if (NextColorCyclingMode >= (uint8)EColorCyclingMode::NumColorCyclingModes)
	//{
	//	NextColorCyclingMode = (uint8)EColorCyclingMode::RedToGreen;
	//}
}

