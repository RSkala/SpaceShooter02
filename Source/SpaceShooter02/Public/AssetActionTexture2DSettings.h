// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "AssetActionTexture2DSettings.generated.h"

// This class contains custom context-menu actions (i.e. right-click an asset in the Content Browser) for Texture2D settings.
// These actions will appear under "Scripted Asset Actions" after right-clicking a Texture2D asset.
UCLASS()
class SPACESHOOTER02_API UAssetActionTexture2DSettings : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UAssetActionTexture2DSettings();

	// The default Paper2D import texture settings asset action doesn't apply all of the settings that need to be changed.
	// This function applies the same settings plus a few in addition.
	// We want to change the following:
	// * Level of Detail -> Mip Gen Settings: NoMipMaps
	// * Level of Detail -> Texture Group: 2D Pixels (unfiltered)
	// * Texture -> Advanced -> Filter: Nearest
	// * Compression -> Compression Settings: UserInterface2D (RGBA)
	// * Texture -> sRGB: True
	UFUNCTION(CallInEditor)
	void ApplyPaper2DTextureSettings();
};
