// Fill out your copyright notice in the Description page of Project Settings.


#include "CHDataTableManager.h"

FDataTableRequestHandle::~FDataTableRequestHandle()
{
	UCHDataTableManager* LocalManager = OwningManager.Get();
	if (LocalManager)
	{
		LocalManager->RemoveDataTableListRequest(Category, DataTableClassList);
	}
}

bool FDataTableRequestHandle::IsValid() const
{
	return OwningManager.IsValid();
}

TSharedPtr<FDataTableRequestHandle> UCHDataTableManager::AddDataTableListRequest(const FGameplayTag& Category,
	TArray<TSubclassOf<UDataTable>> DataTableClassList)
{
	if (!ensure(Category.IsValid()) || !ensure(DataTableClassList.Num() > 0))
	{
		return nullptr;
	}

	FDataTableListRequest NewRequest;
	NewRequest.Category = Category;
	TArray<UClass*> DataTableClassPtrList;
	
	for (TSubclassOf DataTableClass : DataTableClassList)
	{
		UClass* DataTableClassPtr = DataTableClass.Get();
		DataTableClassPtrList.Add(DataTableClassPtr);
	}
	NewRequest.DataTableClassList = DataTableClassPtrList;

	// Add a request if there is not an already existing one. Note that it will only uses the receiver and component class to check for uniqueness, not the addition flags.
	int32& RequestCount = RequestTrackingMap.FindOrAdd(NewRequest);
	RequestCount++;

	if (RequestCount == 1)
	{
		RegisterDataTableList(Category, DataTableClassList);

		return MakeShared<FDataTableRequestHandle>(this, Category, DataTableClassList);
	}

	return nullptr;
}

void UCHDataTableManager::RemoveDataTableListRequest(const FGameplayTag& Category, TArray<TSubclassOf<UDataTable>> DataTableClassList)
{
	FDataTableListRequest NewRequest;
	NewRequest.Category = Category;
	TArray<UClass*> DataTableClassPtrList;
	
	for (TSubclassOf DataTableClass : DataTableClassList)
	{
		UClass* DataTableClassPtr = DataTableClass.Get();
		DataTableClassPtrList.Add(DataTableClassPtr);
	}
	NewRequest.DataTableClassList = DataTableClassPtrList;
	int32& RequestCount = RequestTrackingMap.FindChecked(NewRequest);
	check(RequestCount > 0);
	RequestCount--;

	if (RequestCount == 0)
	{
		UnregisterDataTable(Category);
	}
}

void UCHDataTableManager::RegisterDataTableList(const FGameplayTag& Category, const TArray<TSubclassOf<UDataTable>>& DataTableClassList)
{
	if (!ensure(Category.IsValid()) || !ensure(DataTableClassList.Num() > 0))
	{
		return;
	}

	for (TSubclassOf DataTableClass : DataTableClassList)
	{
		UDataTable* DataTable = DataTableClass->GetDefaultObject<UDataTable>();
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
	RegisteredDataTables.Remove(Category);
}
