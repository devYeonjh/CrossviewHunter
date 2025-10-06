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
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"EditorStyle",
				"AssetTools",
				"PropertyEditor",
				"DetailCustomizations",
			}
		);
	}
}
