// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_CHEquipmentInfo.h"

#include "AbilitySystem/LyraAbilitySet.h"
#include "Equipment/CHEquipmentDefinition.h"
#include "CHItemDataTableRows.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "GameplayEffect.h"

UInventoryFragment_CHEquipmentInfo::UInventoryFragment_CHEquipmentInfo()
{
	EquipmentDef = NewObject<UCHEquipmentDefinition>();
}

void UInventoryFragment_CHEquipmentInfo::InitializeValue(
	const ECHItemType InType,
	const ECHEquipmentSlot InSlot,
	const TMap<TSubclassOf<UGameplayEffect>, float>& DefaultOptions,
	const TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& AdditionalOptions)
{
	Type = InType;
	Slot = InSlot;
	EquipmentDef->SetDefaultOptions(DefaultOptions);
	EquipmentDef->SetAdditionalOptions(AdditionalOptions);
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
