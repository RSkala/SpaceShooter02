// Copyright 2024 Richard Skala

#include "SpaceShooterLevelScriptActor.h"

#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpaceShooterLevelScript, Log, All)

void ASpaceShooterLevelScriptActor::GetLevelBoundingBoxPositionAndExtent(FVector& OutPosition, FVector& OutExtent) const
{
	if (LevelBoundingBox != nullptr)
	{
		OutPosition = LevelBoundingBox->GetActorLocation();
	}
	OutExtent = LevelBoundingBoxExtent;
}

void ASpaceShooterLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	if (ensureAlwaysMsgf(
		LevelBoundingBox != nullptr,
		TEXT("%s - LevelBoundingBox is invalid. This must be set in Level Blueprint."), ANSI_TO_TCHAR(__FUNCTION__)))
	{
		if (UBoxComponent* LevelBoundingBoxComp = Cast<UBoxComponent>(LevelBoundingBox->GetRootComponent()))
		{
			LevelBoundingBoxExtent = LevelBoundingBoxComp->GetUnscaledBoxExtent();
		}
	}
}
