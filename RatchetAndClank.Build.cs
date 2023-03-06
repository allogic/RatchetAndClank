using UnrealBuildTool;

public class RatchetAndClank : ModuleRules
{
	public RatchetAndClank(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
		PublicDependencyModuleNames.AddRange(new string[] { "InputCore", "EnhancedInput" });
		PublicDependencyModuleNames.AddRange(new string[] { "Niagara" });
		PublicDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });
		PublicDependencyModuleNames.AddRange(new string[] { "MassCommon", "MassEntity", "MassCrowd" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
