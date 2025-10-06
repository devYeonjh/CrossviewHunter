// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrossviewHunterEditor.h"
#include "CHDataTableImportHandler.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "CrossviewHunterEditor"

DEFINE_LOG_CATEGORY(LogCrossviewHunterEditor);

/**
 * FCrossviewHunterEditorModule
 */
class FCrossviewHunterEditorModule : public FDefaultGameModuleImpl
{
	typedef FCrossviewHunterEditorModule ThisClass;

	// virtual void StartupModule() override
	// {
	// 	if (!IsRunningGame())
	// 	{
	// 		// DataTable Import Handler 초기화
	// 		FCHDataTableImportHandler::Initialize();
	// 	}
	// }
	//
	// virtual void ShutdownModule() override
	// {
	// 	// DataTable Import Handler 정리
	// 	FCHDataTableImportHandler::Shutdown();
	// }
};

IMPLEMENT_MODULE(FCrossviewHunterEditorModule, CrossviewHunterEditor);

#undef LOCTEXT_NAMESPACE
