// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ColonyWorldGen: ModuleRules
{
	public ColonyWorldGen (ReadOnlyTargetRules Target) : base(Target)
    {
        PublicIncludePaths.AddRange(new string[] { "ColonyWorldGen/Public" });

        PrivateIncludePaths.AddRange(new string[] { "ColonyWorldGen/Private" });

        PublicDependencyModuleNames.AddRange(new string[] { "ColonyBuilder", "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
};
