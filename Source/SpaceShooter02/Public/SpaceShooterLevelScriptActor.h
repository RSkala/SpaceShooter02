// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SpaceShooterLevelScriptActor.generated.h"

UCLASS()
class SPACESHOOTER02_API ASpaceShooterLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	void GetLevelBoundingBoxPositionAndExtent(FVector& OutPosition, FVector& OutExtent) const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintSetter)
	void SetLevelBoundingBox(class ATriggerBox* InLevelBoundingBox) { LevelBoundingBox = InLevelBoundingBox; }

protected:
	// Bounding box for the game level. Enemies should be spawned within this area.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetLevelBoundingBox, Category = "Game Level")
	TObjectPtr<class ATriggerBox> LevelBoundingBox;

	// The Extent of the level bounding box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Level")
	FVector LevelBoundingBoxExtent;

	// If true, skips loading the main menu and goes right into gameplay -- DEPRECATED
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSkipMainMenuLoad = false; // TODO: Move into GameState
};
