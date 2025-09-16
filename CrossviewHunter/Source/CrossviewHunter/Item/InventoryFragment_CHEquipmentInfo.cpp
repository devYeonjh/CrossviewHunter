// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_CHEquipmentInfo.h"

#include "AbilitySystem/LyraAbilitySet.h"
#include "Equipment/CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"

UInventoryFragment_CHEquipmentInfo::UInventoryFragment_CHEquipmentInfo()
{
	EquipmentDef = NewObject<UCHEquipmentDefinition>();
}

void UInventoryFragment_CHEquipmentInfo::InitializeValue(ECHItemType InType, ECHEquipmentSlot InSlot,
                                                         const TMap<ECHStatID, float>& InModifiers)
{
	Type = InType;
	Slot = InSlot;
	EquipmentDef->SetModifiers(InModifiers);
}

void UInventoryFragment_CHEquipmentInfo::SetEquipmentDefinitionByData(const FEquipmentTypeDefinitionRow& DataRow) const
{
	EquipmentDef->InstanceType = DataRow.InstanceType.Get();
	for (TSoftObjectPtr<ULyraAbilitySet> AbilitySet : DataRow.AbilitySetsToGrant)
	{
		EquipmentDef->AbilitySetsToGrant.Add(AbilitySet.Get());
	}
	EquipmentDef->ActorsToSpawn = DataRow.ActorsToSpawn;
}
