// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameplayTagContainer.h"
#include "GameFeaturesSubsystem.h"

#include "GameFeatureAction_AddDataTables.generated.h"

class UCHDataTableManager;
struct FDataTableRequestHandle;
struct FWorldContext;

USTRUCT()
struct FGameFeatureDataTablesEntry
{
	GENERATED_BODY()

	FGameFeatureDataTablesEntry();
	
	UPROPERTY(EditAnywhere, Category="DataTables", meta = (Categories = "CH.DT"))
	FGameplayTag Category;
	
	UPROPERTY(EditAnywhere, Category="DataTables")
	uint8 bClientData:1;

	UPROPERTY(EditAnywhere, Category="DataTables")
	uint8 bServerData:1;

	UPROPERTY(EditAnywhere, Category="DataTables")
	TArray<TSoftObjectPtr<UDataTable>> DataTables;
};

/**
 * GameDataTable 매니저에 데이터 테이블을 등록하는 GameFeatureAction
 * UGameFeatureAction_AddComponents 구조를 기반으로 작성됨
 *
 * @see UCHDataTableManager
 */
UCLASS(meta = (DisplayName = "Add DataTables"))
class CROSSVIEWHUNTER_API UGameFeatureAction_AddDataTables : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface

	UPROPERTY(EditAnywhere, Category="DataTables")
	TArray<FGameFeatureDataTablesEntry> DataTableList;

private:
	struct FContextHandles
	{
		FDelegateHandle GameInstanceStartHandle;
		TArray<TSharedPtr<FDataTableRequestHandle>> DataTableRequestHandles;
	};

	void AddToWorld(const FWorldContext& WorldContext, FContextHandles& Handles);

	void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);

	TMap<FGameFeatureStateChangeContext, FContextHandles> ContextHandles;
};
