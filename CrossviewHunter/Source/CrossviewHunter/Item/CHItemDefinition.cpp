// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"
#include "InventoryFragment_CHEquipmentInfo.h"

void UCHItemDefinition::SetItemData(const FItemDataTableRow& DataTableRow)
{
	DisplayName = DataTableRow.ItemName;
	TObjectPtr<UInventoryFragment_CHEquipmentInfo> EquipInfo = NewObject<UInventoryFragment_CHEquipmentInfo>();
	EquipInfo->InitializeValue(DataTableRow.ItemType, DataTableRow.EquipmentSlot, DataTableRow.BaseStats);
	Fragments.Add(EquipInfo);

	//@TODO: Fragment List 데이터테이블에 추가
}
