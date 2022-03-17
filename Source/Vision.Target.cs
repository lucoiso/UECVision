// Developer: Lucas Vilas-Boas
// Year: 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class VisionTarget : TargetRules
{
	public VisionTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Vision" } );
	}
}
