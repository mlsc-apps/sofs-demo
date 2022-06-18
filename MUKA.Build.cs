// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MUKA : ModuleRules
{
	public MUKA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // PublicDependencyModuleNames.AddRange(new string[] { "StoryOfUs" });

        PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "UMG", "LevelSequence", "MovieScene" });

        PrivateDependencyModuleNames.AddRange(new string[] {
						"UnrealEd",
                        "KismetCompiler",
                        "BlueprintGraph",
						"GraphEditor",
						"SlateCore",
						"Slate"
    });
        // PrivateDependencyModuleNames.AddRange(new string[] {  });

        // PrivateDependencyModuleNames.AddRange(
        //     new string[]
        //     {
        //         "UnrealEd",
        //         "KismetCompiler"
        //     });
		
		// PublicIncludePaths.AddRange(
		// 	new string[] {
		// 		// ... add public include paths required here ...
		// 	}
		// 	);
				
		
		// PrivateIncludePaths.AddRange(
		// 	new string[] {
		// 		// ... add other private include paths required here ...
		// 	}
		// 	);
			
		
		// PublicDependencyModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 		"Core",
		// 		// ... add other public dependencies that you statically link with here ...
		// 	}
		// 	);
			
		
		// PrivateDependencyModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 		"CoreUObject",
		// 		"Engine",
		// 		"Slate",
		// 		"SlateCore",
		// 		// ... add private dependencies that you statically link with here ...	
		// 	}
		// 	);
		
		
		// DynamicallyLoadedModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 		// ... add any modules that your module loads dynamically here ...
		// 	}
		// 	);
	}
}
