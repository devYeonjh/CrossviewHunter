// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CrossviewHunterEditorTarget : TargetRules
{
	public CrossviewHunterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new string[] { "CrossviewHunter","LyraGame", "LyraEditor", "CrossviewHunterEditor" });

		if (!bBuildAllModules)
		{
			NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
		}

		CrossviewHunterTarget.ApplySharedCrossviewHunterTargetSettings(this);

		// This is used for touch screen development along with the "Unreal Remote 2" app
		EnablePlugins.Add("RemoteSession");
	}
}
