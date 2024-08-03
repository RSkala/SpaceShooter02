// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceShooterGameInstance.generated.h"

UCLASS()
class SPACESHOOTER02_API USpaceShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static FString GetGameVersionString() { return GameVersion; }

protected:
	virtual void OnStart() override;

private:
	static FString GameVersion;
};
