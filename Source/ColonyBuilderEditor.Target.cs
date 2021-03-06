// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ColonyBuilderEditorTarget : TargetRules
{
	public ColonyBuilderEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ColonyBuilder", "ColonyAI" } );
		
		        // Game editor
        ExtraModuleNames.AddRange(
            new string[]
            {
               "ColonyEditor"
            });
	}
}
