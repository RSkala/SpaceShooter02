// Copyright 2024 Richard Skala

#include "AssetActionTexture2DSettings.h"
#include "EditorUtilityLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogAssetActionTexture2DSettings, Log, All)

UAssetActionTexture2DSettings::UAssetActionTexture2DSettings()
{
	SupportedClasses.Add(UTexture2D::StaticClass());
}

void UAssetActionTexture2DSettings::ApplyPaper2DTextureSettings()
{
	UEditorUtilityLibrary::GetSelectedAssets();

	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	for (UObject* CurSelectedAsset : SelectedAssets)
	{
		if (UTexture2D* Texture2D = Cast<UTexture2D>(CurSelectedAsset))
		{
			UE_LOG(LogAssetActionTexture2DSettings, Log, TEXT("Applying texture settings to Texture2D '%s'"), *Texture2D->GetName());

			// Mark the Texture2D as dirty. Note: In Blueprint you would have to call UKismetSystemLibrary::TransactObject();
			Texture2D->Modify();

			// Apply the desired 2D texture settings
			Texture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			Texture2D->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;
			Texture2D->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
			Texture2D->Filter = TextureFilter::TF_Nearest;
			Texture2D->SRGB = true;

			// Apply the changes to the Texture2D asset
			Texture2D->PostEditChange();
		}
	}
}
