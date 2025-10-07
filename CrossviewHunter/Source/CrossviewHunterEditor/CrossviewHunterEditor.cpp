// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrossviewHunterEditor.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "CrossviewHunterEditor"

DEFINE_LOG_CATEGORY(LogCrossviewHunterEditor);

/**
 * FCrossviewHunterEditorModule
 */
class FCrossviewHunterEditorModule : public FDefaultGameModuleImpl
{
	typedef FCrossviewHunterEditorModule ThisClass;

	virtual void StartupModule() override
	{
		if (!IsRunningGame())
		{
			
		}
	}
	
	virtual void ShutdownModule() override
	{
		
	}
};

IMPLEMENT_MODULE(FCrossviewHunterEditorModule, CrossviewHunterEditor);

#undef LOCTEXT_NAMESPACE
