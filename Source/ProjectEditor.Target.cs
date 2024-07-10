// Copyright 2020-2021 CesiumGS, Inc. and Contributors

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectEditorTarget : TargetRules
{
	public ProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Project" } );
	}
}
