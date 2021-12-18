// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class pic : ModuleRules
{
	public pic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "GameplayTasks", "NavigationSystem" });
	}
}
