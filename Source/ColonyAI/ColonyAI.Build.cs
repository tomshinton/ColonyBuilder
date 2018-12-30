// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ColonyAI: ModuleRules
{

	public ColonyAI (ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ColonyAI/Public" });

        PrivateIncludePaths.AddRange(new string[] { "ColonyAI/Private" });

        PublicDependencyModuleNames.AddRange(new string[] { "ColonyBuilder", "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
};
