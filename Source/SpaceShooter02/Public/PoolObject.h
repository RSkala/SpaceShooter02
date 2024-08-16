// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolObject.generated.h"

// This class does not need to be modified.
UINTERFACE()
class SPACESHOOTER02_API UPoolObject : public UInterface
{
	GENERATED_BODY()
};

class SPACESHOOTER02_API IPoolObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivatePoolObject() = 0;
	virtual void DeactivatePoolObject() = 0;
	virtual bool IsPoolObjectActive() const = 0;
};
