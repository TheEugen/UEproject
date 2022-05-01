// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG_cpp : ModuleRules
{
	public RPG_cpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "XmlParser", "AIModule", "SlateCore" });
	}
}
