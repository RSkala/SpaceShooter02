// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBorder.generated.h"

// A level border defines the border of the playable area. A player cannot cross a LevelBorder.
UCLASS()
class SPACESHOOTER02_API ALevelBorder : public AActor
{
	GENERATED_BODY()
	
public:
	ALevelBorder();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnColorShift(FLinearColor LinearColor);

protected:

	// --- Components ---

	// Box collision for border
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> BoxComp;

	// Visible sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;
};
