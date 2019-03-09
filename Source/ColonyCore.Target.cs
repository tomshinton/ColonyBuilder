using UnrealBuildTool;
using System.Collections.Generic;

public class ColonyCoreTarget : TargetRules
{
	public ColonyCoreTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
	
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
