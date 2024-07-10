// Copyright 2020-2021 CesiumGS, Inc. and Contributors

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectTarget : TargetRules
{
	public ProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Project" } );
	}
}
