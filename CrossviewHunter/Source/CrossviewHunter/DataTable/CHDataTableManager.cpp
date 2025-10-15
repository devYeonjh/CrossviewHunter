// Fill out your copyright notice in the Description page of Project Settings.


#include "CHDataTableManager.h"

FDataTableRequestHandle::~FDataTableRequestHandle()
{
	UCHDataTableManager* LocalManager = OwningManager.Get();
	if (LocalManager)
	{
		LocalManager->RemoveDataTableListRequest(Category, DataTableList);
	}
}

bool FDataTableRequestHandle::IsValid() const
{
	return OwningManager.IsValid();
}

TSharedPtr<FDataTableRequestHandle> UCHDataTableManager::AddDataTableListRequest(const FGameplayTag& Category,
	const TArray<UDataTable*>& DataTableList)
{
	if (!ensure(Category.IsValid()) || !ensure(DataTableList.Num() > 0))
	{
		return nullptr;
	}

	FDataTableListRequest NewRequest;
	NewRequest.Category = Category;
	NewRequest.DataTableList = DataTableList;

	// Add a request if there is not an already existing one. Note that it will only uses the receiver and component class to check for uniqueness, not the addition flags.
	int32& RequestCount = RequestTrackingMap.FindOrAdd(NewRequest);
	RequestCount++;

	if (RequestCount == 1)
	{
		RegisterDataTableList(Category, DataTableList);

		return MakeShared<FDataTableRequestHandle>(this, Category, DataTableList);
	}

	return nullptr;
}

TArray<UDataTable*> UCHDataTableManager::GetDataTableList(const FGameplayTag& Category) const
{
	if (RegisteredDataTables.Contains(Category))
	{
		return RegisteredDataTables.FindRef(Category);
	}
	return TArray<UDataTable*>();
}

void UCHDataTableManager::RemoveDataTableListRequest(const FGameplayTag& Category, const TArray<UDataTable*>& DataTableList)
{
	FDataTableListRequest NewRequest;
	NewRequest.Category = Category;
	NewRequest.DataTableList = DataTableList;
	int32& RequestCount = RequestTrackingMap.FindChecked(NewRequest);
	check(RequestCount > 0);
	RequestCount--;

	if (RequestCount == 0)
	{
		UnregisterDataTable(Category);
	}
}

void UCHDataTableManager::RegisterDataTableList(const FGameplayTag& Category, const TArray<UDataTable*>& DataTableList)
{
	if (!ensure(Category.IsValid()) || !ensure(DataTableList.Num() > 0))
	{
		return;
	}

	for (UDataTable* DataTable : DataTableList)
	{
		if (!ensure(DataTable))
		{
			return;
		}
		TArray<UDataTable*>& DataTablesLoaded = RegisteredDataTables.FindOrAdd(Category);
		DataTablesLoaded.Emplace(DataTable);
	}
}


void UCHDataTableManager::UnregisterDataTable(const FGameplayTag& Category)
{
	for (UDataTable* DataTable : RegisteredDataTables.FindRef(Category))
	{
		// UDataTable은 UObject 기반이므로 nullptr로 초기화하여 GC가 자동으로 처리하도록 설정
		DataTable = nullptr;
	}
	RegisteredDataTables.Remove(Category);
}
