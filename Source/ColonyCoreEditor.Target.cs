using UnrealBuildTool;
using System.Collections.Generic;

public class ColonyCoreEditorTarget : TargetRules
{
	public ColonyCoreEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		
		 // Game editor
        	ExtraModuleNames.AddRange(
           	new string[]
            	{
              		"ColonyEditor"
            	});
	}
}
