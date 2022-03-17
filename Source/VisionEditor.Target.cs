// Developer: Lucas Vilas-Boas
// Year: 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class VisionEditorTarget : TargetRules
{
	public VisionEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Vision" } );
	}
}
