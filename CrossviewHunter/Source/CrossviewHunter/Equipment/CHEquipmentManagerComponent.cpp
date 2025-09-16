// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentManagerComponent.h"

#include "CHEquipmentDefinition.h"
#include "Item/CHItemInstance.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "Item/InventoryFragment_CHEquipmentInfo.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "Character/LyraCharacter.h"


//////////////////////////////////////////////////////////////////////
// UCHEquipmentManagerComponent

ULyraEquipmentInstance* FCHEquipmentList::AddEntry(UCHEquipmentDefinition* EquipmentDefinition)
{
	ULyraEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	

	TSubclassOf<ULyraEquipmentInstance> InstanceType = EquipmentDefinition->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = ULyraEquipmentInstance::StaticClass();
	}

	// 다운 캐스팅
	FCHAppliedEquipmentEntry& NewEntry = static_cast<FCHAppliedEquipmentEntry&>(Entries.AddDefaulted_GetRef());
	NewEntry.CHEquipmentDef = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentDefinition->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
		//Modifiers 실행
		if (ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(OwnerComponent->GetOwner()))
		{
			EquipmentDefinition->ApplyModifiers(LyraCharacter);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentDefinition->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FCHEquipmentList::RemoveEntry(ULyraEquipmentInstance* Instance)
{
	FLyraEquipmentList::RemoveEntry(Instance);
	
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		//다운 캐스팅
		FCHAppliedEquipmentEntry& Entry = static_cast<FCHAppliedEquipmentEntry&>(*EntryIt);
		if (Entry.Instance == Instance)
		{
			if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
				Entry.CHEquipmentDef->RemoveModifiers();
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

ULyraEquipmentInstance* UCHEquipmentManagerComponent::EquipItemInstance(UCHItemInstance* ItemInstance)
{
	//EquipInfo Fragment 가져오기
	const UInventoryFragment_CHEquipmentInfo* EquipInfoFragment = ItemInstance->FindFragmentByClass<UInventoryFragment_CHEquipmentInfo>();

	if  (EquipInfoFragment == nullptr)
		return nullptr;
	
	//fragment에 설정된 Definition
	UCHEquipmentDefinition* EquipmentDef= EquipInfoFragment->GetEquipmentDef();
	
	ULyraEquipmentInstance* Result = nullptr;
	FCHEquipmentList* CHEquipmentList = static_cast<FCHEquipmentList*>(&EquipmentList);
	if (EquipmentDef != nullptr)
	{
		Result = CHEquipmentList->AddEntry(EquipmentDef);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
			
		}
	}
	return Result;
}

void UCHEquipmentManagerComponent::UnequipItem(ULyraEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		
		static_cast<FCHEquipmentList>(EquipmentList).RemoveEntry(ItemInstance);
		
	}
}