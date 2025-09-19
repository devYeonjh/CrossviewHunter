// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"
#include "InventoryFragment_CHEquipmentInfo.h"
#include "CHItemCreationComponent.h"
#include "CHOptionPool.h"


void UCHItemDefinition::SetItemData(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	ItemID = DataTableRow.ItemID;
	DisplayName = DataTableRow.ItemName;
	
	SetEquipmentFragment(ICC, DataTableRow);
}

void UCHItemDefinition::SetEquipmentFragment(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	TObjectPtr<UInventoryFragment_CHEquipmentInfo> EquipInfo = NewObject<UInventoryFragment_CHEquipmentInfo>();

	TMap<TSubclassOf<UGameplayEffect>, float> DefaultOptions;
	TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions;

	// Default 옵션 추가
	for (auto It = DataTableRow.BaseStats.CreateConstIterator(); It; ++It)
	{
		DefaultOptions.Add(ICC->GetStatEffect(It.Key()),It.Value());
	}
	
	// Additional 옵션 추가
	TObjectPtr<UCHOptionPool> OptionPool = ICC->GetOptionPool(DataTableRow.OptionPoolID);
	TArray<TPair<ECHStatID, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> RandomStatArray = OptionPool->GetRandomOptions(DataTableRow.ItemGrade);

	for (TPair RandomStat : RandomStatArray)
	{
		AdditionalOptions.Emplace(TPair<TSubclassOf<UGameplayEffect>, float>(ICC->GetStatEffect(RandomStat.Key), RandomStat.Value));
	}

	// EquipInfo fragment에서 값 설정
	EquipInfo->InitializeValue(DataTableRow.ItemType, DataTableRow.EquipmentSlot, DefaultOptions, AdditionalOptions);
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
