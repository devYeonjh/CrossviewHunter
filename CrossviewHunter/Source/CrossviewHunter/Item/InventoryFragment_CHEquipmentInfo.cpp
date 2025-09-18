// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_CHEquipmentInfo.h"

#include "AbilitySystem/LyraAbilitySet.h"
#include "Equipment/CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "GameplayEffect.h"

UInventoryFragment_CHEquipmentInfo::UInventoryFragment_CHEquipmentInfo()
{
	EquipmentDef = NewObject<UCHEquipmentDefinition>();
}

void UInventoryFragment_CHEquipmentInfo::InitializeValue(ECHItemType InType, ECHEquipmentSlot InSlot,
                                                         TMap<TSubclassOf<UGameplayEffect>, float> InModifiers)
{
	Type = InType;
	Slot = InSlot;
	EquipmentDef->SetModifiers(InModifiers);
}

void UInventoryFragment_CHEquipmentInfo::SetEquipmentDefinitionByData(const FCHEquipmentTypeDefinitionRow& DataRow) const
{
	EquipmentDef->InstanceType = DataRow.InstanceType.Get();
	for (TSoftObjectPtr<ULyraAbilitySet> AbilitySet : DataRow.AbilitySetsToGrant)
	{
		EquipmentDef->AbilitySetsToGrant.Add(AbilitySet.Get());
	}
	EquipmentDef->ActorsToSpawn = DataRow.ActorsToSpawn;
}
