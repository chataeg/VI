// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VI : ModuleRules
{
	public VI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		// publicPath Ãß°¡
        PublicIncludePaths.AddRange(new string[] { "VI" });


    }
}
