// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrossviewHunterEditor : ModuleRules
{
	public CrossviewHunterEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"CrossviewHunterEditor"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"EditorFramework",
				"UnrealEd",
				"GameplayTags",
				"GameplayTagsEditor",
				"CrossviewHunter",
				"LyraGame",
				"PropertyEditor",
				"EditorSubsystem" 
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"InputCore",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"EditorStyle",
				"AssetTools",
				"PropertyEditor",
				"DetailCustomizations",
				"EditorWidgets",       
				"Blutility",
				"UMG",
				"Landscape",
				"PropertyEditor",
				"EditorSubsystem" 
			}
		);
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("EditorScriptingUtilities");
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}
