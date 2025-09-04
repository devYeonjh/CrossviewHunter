// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFeatures/GameFeatureAction_AddDataTable.h"

FItemStatTableRow UGameFeatureAction_AddDataTable::GetItemStatByID(int32 ItemID, const FString& TableKey) const
{
	FItemStatTableRow Result;
	FindItemStatByID(ItemID, Result, TableKey);
	return Result;
}

bool UGameFeatureAction_AddDataTable::FindItemStatByID(int32 ItemID, FItemStatTableRow& OutItemStat, const FString& TableKey) const
{
	if (ItemStatTables.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemStatTables map is empty - No data tables configured"));
		return false;
	}

	const TSoftObjectPtr<UDataTable>* TablePtrPtr = ItemStatTables.Find(TableKey);
	if (!TablePtrPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("TableKey '%s' not found in ItemStatTables map"), *TableKey);
		return false;
	}

	const TSoftObjectPtr<UDataTable>& TablePtr = *TablePtrPtr;
	
	if (!TablePtr.IsValid())
	{
		UDataTable* LoadedTable = TablePtr.LoadSynchronous();
		if (!LoadedTable)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load ItemStatTable[%s] - DataTable reference is invalid or null"), *TableKey);
			return false;
		}
	}

	UDataTable* Table = TablePtr.Get();
	if (!Table)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemStatTable[%s] is null - DataTable failed to load"), *TableKey);
		return false;
	}

	FString RowName = FString::FromInt(ItemID);
	FItemStatTableRow* FoundRow = Table->FindRow<FItemStatTableRow>(FName(*RowName), TEXT(""));
	
	if (FoundRow)
	{
		OutItemStat = *FoundRow;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Item with ID %d not found in StatTable[%s]"), ItemID, *TableKey);
	return false;
}
