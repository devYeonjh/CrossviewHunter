// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"
#include "InventoryFragment_CHEquipmentInfo.h"
#include "CHItemCreationComponent.h"
#include "CHItemDataTableRows.h"
#include "CHOptionPool.h"


void UCHItemDefinition::SetItemData(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	ItemID = DataTableRow.ItemID;
	DisplayName = DataTableRow.ItemName;
	
	SetEquipmentFragment(ICC, DataTableRow);
}

void UCHItemDefinition::SetEquipmentFragment(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	TMap<TSubclassOf<UGameplayEffect>, int32> DefaultOptions;
	TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions;

	// Default 옵션 추가
	for (auto It = DataTableRow.BaseStats.CreateConstIterator(); It; ++It)
	{
		DefaultOptions.Add(ICC->GetStatEffect(It.Key()),It.Value());
	}
	
	// Additional 옵션 추가
	TObjectPtr<UCHOptionPool> OptionPool = ICC->GetOptionPool(DataTableRow.OptionPoolID);
	TArray<FCHItemOptionDetailRow> OptionDetails;
	TArray<TPair<ECHStatID, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> RandomStatArray = OptionPool->GetRandomOptionsWithInfos(DataTableRow.ItemGrade, OptionDetails);

	for (TPair RandomStat : RandomStatArray)
	{
		AdditionalOptions.Emplace(TPair<TSubclassOf<UGameplayEffect>, int32>(ICC->GetStatEffect(RandomStat.Key), RandomStat.Value));
	}

	
	// EquipInfo fragment에서 값 설정
	TObjectPtr<UInventoryFragment_CHEquipmentInfo> EquipInfo = NewObject<UInventoryFragment_CHEquipmentInfo>();
	int32 DecomposeItemCount = OptionPool->GetDecomposeItemCount(DataTableRow.ItemGrade);
	EquipInfo->InitializeValue(DataTableRow, DefaultOptions, AdditionalOptions, OptionDetails, DecomposeItemCount);
	FCHEquipmentTypeDefinitionRow EquipTypeDefinition = ICC->FindEquipmentTypeDefinition(DataTableRow.ItemType);
	EquipInfo->SetEquipmentDefinitionByData(EquipTypeDefinition);
	Fragments.Add(EquipInfo);


	// 기본 장비 Definition(WID)에 잇는 Fragment들 추가
	if (UClass* DefinitionClass = EquipTypeDefinition.BaseItemDefinition.LoadSynchronous())
	{
		if (ULyraInventoryItemDefinition* DefaultObject = DefinitionClass->GetDefaultObject<ULyraInventoryItemDefinition>())
		{
			Fragments.Append(DefaultObject->Fragments);
		}
	}
}
