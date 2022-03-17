// Developer: Lucas Vilas-Boas
// Year: 2022

using UnrealBuildTool;

public class Vision : ModuleRules
{
	public Vision(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"ImageWriteQueue",
			"HTTP",
			"Json"
			});
	}
}
