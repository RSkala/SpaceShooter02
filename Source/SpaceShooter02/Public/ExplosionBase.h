// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionBase.generated.h"

// Class for playing an enemy explosion PaperFlipbook animation.
// Using a PaperFlipBookComponent is the only way to be notified when a
// PaperFlipbook animation finishes, unless you use the PaperZD plugin.
UCLASS(Abstract)
class SPACESHOOTER02_API AExplosionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosionBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnExplosionAnimationFinished();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> ExplosionFlipbookComp;
};
