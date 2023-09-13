// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CarpenterDemo : ModuleRules
{
	public CarpenterDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
