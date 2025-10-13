// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrossviewHunterEditor.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "CHMonsterSpawnPointRowCustomization.h"
 

/**
 * FCrossviewHunterEditorModule
 */
class FCrossviewHunterEditorModule : public FDefaultGameModuleImpl
{
	typedef FCrossviewHunterEditorModule ThisClass;

	virtual void StartupModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(
			"CHMonsterSpawnPointRow", // USTRUCT 이름 정확히 입력
			FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCHMonsterSpawnPointRowCustomization::MakeInstance)
		);
	}
	
	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout("CHMonsterSpawnPointRow");
		}
	}
};

IMPLEMENT_MODULE(FCrossviewHunterEditorModule, CrossviewHunterEditor)