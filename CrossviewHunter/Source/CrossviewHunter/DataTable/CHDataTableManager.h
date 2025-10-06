// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "CHDataTableManager.generated.h"

class UCHDataTableManager;

struct FDataTableRequestHandle
{
	FDataTableRequestHandle(const TWeakObjectPtr<UCHDataTableManager>& InOwningManager, const FGameplayTag& InCategory, const TArray<TSubclassOf<UDataTable>>& InDataTableClassList)
		: OwningManager(InOwningManager)
		, Category(InCategory)
		, DataTableClassList(InDataTableClassList)
	{}

	~FDataTableRequestHandle();

	/** Returns true if the manager that this request is for still exists */
	bool IsValid() const;
	
	/** 요청 대상 매니저 */
	TWeakObjectPtr<UCHDataTableManager> OwningManager;

	/** DataTable 카테고리 */
	FGameplayTag Category;
	
	/** DataTable 클래스 */
	TArray<TSubclassOf<UDataTable>> DataTableClassList;
};


/**
 * GameDataTable 매니저
 *
 * DataTable을 관리하는 서브시스템.
 * GameFrameworkComponentManager의 구조를 기반으로 작성됨
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHDataTableManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/**
	 * DataTable을 등록하는 요청을 추가합니다.
	 *
	 * @param Category			DataTable이 포함되는 카테고리 태그
	 * @param DataTableClass	DataTable 클래스
	 * 
	 * @return 요청 핸들, 요청이 이미 존재하는 경우 nullptr
	 */
	TSharedPtr<FDataTableRequestHandle> AddDataTableListRequest(const FGameplayTag& Category, TArray<TSubclassOf<UDataTable>> DataTableClassList);


private:
	/** FDataTableRequestHandle이 소멸될 때 호출되어 등록된 DataTable 요청을 제거합니다. */
	void RemoveDataTableListRequest(const FGameplayTag& Category, TArray<TSubclassOf<UDataTable>> DataTableClassList);
	
	void RegisterDataTableList(const FGameplayTag& Category, const TArray<TSubclassOf<UDataTable>>& DataTableClassList);
	void UnregisterDataTable(const FGameplayTag& Category);

	/** DataTable 요청 정보 */
	struct FDataTableListRequest
	{
		FGameplayTag Category;
		TArray<UClass*> DataTableClassList;
		
		bool operator==(const FDataTableListRequest& Other) const
		{
			return Category == Other.Category && DataTableClassList == Other.DataTableClassList;
		}

		friend FORCEINLINE uint32 GetTypeHash(const FDataTableListRequest& Request)
		{
			return GetTypeHash(Request.Category) ^ GetTypeHash(Request.DataTableClassList);
		}
	};

	/** 활성화된 모든 DataTable 요청 목록. 여러 곳에서 요청해도 같은 DataTable이 중복으로 등록되지 않도록 합니다 */
	TMap<FDataTableListRequest, int32> RequestTrackingMap;

	/** 등록된 DataTable 목록 */
	TMap<FGameplayTag, TArray<UDataTable*>> RegisteredDataTables;

	friend struct FDataTableRequestHandle;
};
