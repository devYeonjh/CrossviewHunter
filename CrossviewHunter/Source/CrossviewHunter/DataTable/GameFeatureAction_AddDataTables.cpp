// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddDataTables.h"

#include "CHDataTableManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "GameFeatures"


FGameFeatureDataTablesEntry::FGameFeatureDataTablesEntry()
	: bClientData(true)
	, bServerData(true)
{
}


void UGameFeatureAction_AddDataTables::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);

	// 새로 생성되는 게임 인스턴스에 대한 데이터 테이블 등록을 처리하기 위한 핸들
	Handles.GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
		&UGameFeatureAction_AddDataTables::HandleGameInstanceStart, FGameFeatureStateChangeContext(Context));

	ensure(Handles.DataTableRequestHandles.Num() == 0);

	// 게임 인스턴스와 연결된 월드에 데이터 테이블을 등록
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			AddToWorld(WorldContext, Handles);
		}
	}
}

void UGameFeatureAction_AddDataTables::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);

	FWorldDelegates::OnStartGameInstance.Remove(Handles.GameInstanceStartHandle);

	// 핸들을 해제하면 DataTableManager로부터 DataTable들도 제거됩니다
	Handles.DataTableRequestHandles.Empty();
}


#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddDataTables::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsInitialized())
	{
		for (const FGameFeatureDataTablesEntry& Entry : DataTableList)
		{
			if (Entry.bClientData)
			{
				for (TSoftObjectPtr<UDataTable> DataTable : Entry.DataTables)
				{
					AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, DataTable.ToSoftObjectPath().GetAssetPath());
				}
			}
			if (Entry.bServerData)
			{
				for (TSoftObjectPtr<UDataTable> DataTable : Entry.DataTables)
				{
					AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, DataTable.ToSoftObjectPath().GetAssetPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddDataTables::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const FGameFeatureDataTablesEntry& Entry : DataTableList)
	{
		if (!Entry.Category.IsValid())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasInvalidCategory", "Invalid Category at index {0} in DataTableList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.DataTables.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNoDataTables", "Index {0} in DataTableList will do nothing (no data tables)"), FText::AsNumber(EntryIndex)));
		}

		int32 DataTableIndex = 0;
		for (TSoftObjectPtr<UDataTable> DataTable : Entry.DataTables)
		{
			if (DataTable.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullDataTable", "Null DataTable at index {0} in DataTableList[{1}].DataTables"), FText::AsNumber(DataTableIndex), FText::AsNumber(EntryIndex)));
			}
			++DataTableIndex;
		}

		++EntryIndex;
	}

	return Result;
}
#endif

void UGameFeatureAction_AddDataTables::AddToWorld(const FWorldContext& WorldContext, FContextHandles& Handles)
{
	UWorld* World = WorldContext.World();
    UGameInstance* GameInstance = WorldContext.OwningGameInstance;
 
    if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
    {
	    if (UCHDataTableManager* DataTableManager = GameInstance->GetSubsystem<UCHDataTableManager>())
	    {
	    	const ENetMode NetMode = World->GetNetMode();
	    	const bool bIsServer = NetMode != NM_Client;
	    	const bool bIsClient = NetMode != NM_DedicatedServer;
 
	    	UE_LOG(LogTemp, Verbose, TEXT("Registering DataTables for %s on world %s (client:%d, server:%d)"),
				*GetPathNameSafe(this), *World->GetDebugDisplayName(), bIsClient ? 1 : 0, bIsServer ? 1 : 0);
	    	
	    	for (const FGameFeatureDataTablesEntry& Entry : DataTableList)
	    	{
	    		const bool bShouldAddRequest = (bIsServer && Entry.bServerData) || (bIsClient && Entry.bClientData);
	    		if (bShouldAddRequest)
	    		{
	    			if (Entry.Category.IsValid())
	    			{
	    				UE_SCOPED_ENGINE_ACTIVITY(TEXT("Adding DataTables to world %s (%s)"), *World->GetDebugDisplayName(), *Entry.Category.ToString());
	    				TArray<TObjectPtr<UDataTable>> DataTablePtrList;
	    				for (TSoftObjectPtr<UDataTable> DTIT : Entry.DataTables)
	    				{
	    					TObjectPtr<UDataTable> DataTablePtr = DTIT.LoadSynchronous();
	    					if (DataTablePtr)
	    					{
	    						DataTablePtrList.Add(DataTablePtr);
	    					}
	    				}
	    				if (!DataTablePtrList.IsEmpty())
	    				{
	    					Handles.DataTableRequestHandles.Add(DataTableManager->AddDataTableListRequest(Entry.Category, DataTablePtrList));
	    				}
	    			}
	    		}
	    	}
	    }
    }
	
}

void UGameFeatureAction_AddDataTables::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
{
	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		if (ChangeContext.ShouldApplyToWorldContext(*WorldContext))
		{
			FContextHandles* Handles = ContextHandles.Find(ChangeContext);
			if (ensure(Handles))
			{
				AddToWorld(*WorldContext, *Handles);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE