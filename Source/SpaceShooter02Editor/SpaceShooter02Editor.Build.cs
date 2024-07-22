// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceShooter02Editor : ModuleRules
{
	public SpaceShooter02Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"SpaceShooter02"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				//"UnrealEd",
				//"Slate",
				//"SlateCore"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[] 
			{
			});
	}
}
