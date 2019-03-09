// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ColonyEditor : ModuleRules
{
	public ColonyEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ColonyEditor/Public" });

        PrivateIncludePaths.AddRange(new string[] { "ColonyEditor/Private" });

        PublicDependencyModuleNames.AddRange(new string[] { "ColonyBuilder", "ColonyCore", "Core", "CoreUObject", "Engine", "InputCore"});

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
};
