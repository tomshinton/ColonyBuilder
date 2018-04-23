// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ColonyBuilderTarget : TargetRules
{
	public ColonyBuilderTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ColonyBuilder", "ColonyWorldGen" } );
		
		if(Type == TargetType.Editor)
		{
			ExtraModuleNames.AddRange( 
            new string[]
            {
				"ColonyEditor"
            });
        }
	}
}
