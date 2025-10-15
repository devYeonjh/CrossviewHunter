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
	const FCHItemDataTableRow& ItemDataRow,
	const TMap<TSubclassOf<UGameplayEffect>, int32>& DefaultOptions,
	const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& AdditionalOptions,
	const TArray<FCHItemOptionDetailRow>& OptionDetails,
	int32 DecomposeItemCount)
{
	ItemData = ItemDataRow;
	EquipmentDef->SetDefaultOptions(DefaultOptions);
	EquipmentDef->SetAdditionalOptions(AdditionalOptions);
	EquipmentDef->SetOptionDetails(OptionDetails);
	TSubclassOf<ULyraInventoryItemDefinition> ID_Scrap = LoadClass<ULyraInventoryItemDefinition>(nullptr, TEXT("/Game/CrossviewHunter/Item/ID_Scrap.ID_Scrap_C"));
	DecomposeItemDefinition = ID_Scrap;
	DecomposeItemAmount = DecomposeItemCount;
}

void UInventoryFragment_CHEquipmentInfo::ChangeAdditionalOption(
	const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<3>>& AdditionalOptions,
	const TArray<FCHItemOptionDetailRow>& OptionDetails) const
{
	EquipmentDef->SetAdditionalOptions(AdditionalOptions);
	EquipmentDef->SetOptionDetails(OptionDetails);
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
