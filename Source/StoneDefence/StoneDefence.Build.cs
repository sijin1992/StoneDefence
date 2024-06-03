// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StoneDefence : ModuleRules
{
	public StoneDefence(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//系统模块
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
            "Slate",
            "SlateCore",
            "UMG",
			"ApexDestruction",
            "AIModule",
			"GameplayTasks",
            "RawMesh"
        });
		//自定义插件
        PublicDependencyModuleNames.AddRange(new string[] {
            "StoneToolClass", 
			"SimpleScreenLoading", 
			"SimpleTutoria", 
			"SimpleGameSettings", 
			"SimpleArchives",
			"SimpleDrawText"
        });

		//如果是编辑器，才包含的模块
		if (Target.bBuildEditor == true)
		{
            PrivateDependencyModuleNames.AddRange(new string[] {
				"Persona"
			});
        }

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
