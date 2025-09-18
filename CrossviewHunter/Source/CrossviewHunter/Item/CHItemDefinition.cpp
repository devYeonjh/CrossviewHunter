// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"
#include "InventoryFragment_CHEquipmentInfo.h"
#include "CHItemCreationComponent.h"


void UCHItemDefinition::SetItemData(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	ItemID = DataTableRow.ItemID;
	DisplayName = DataTableRow.ItemName;
	
	SetEquipmentFragment(ICC, DataTableRow);
	TSubclassOf<UCHOptionPool> OptionPool = ICC->GetOptionPool(DataTableRow.OptionPoolID);
}

void UCHItemDefinition::SetEquipmentFragment(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow)
{
	TObjectPtr<UInventoryFragment_CHEquipmentInfo> EquipInfo = NewObject<UInventoryFragment_CHEquipmentInfo>();

	TMap<TSubclassOf<UGameplayEffect>, float> Modifiers;

	// Modifier 추가
	for (auto It = DataTableRow.BaseStats.CreateConstIterator(); It; ++It)
	{
		Modifiers.Add(ICC->GetStatEffect(It.Key()),It.Value());
	}

	// EquipInfo fragment에서 값 설정
	EquipInfo->InitializeValue(DataTableRow.ItemType, DataTableRow.EquipmentSlot, Modifiers);
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
