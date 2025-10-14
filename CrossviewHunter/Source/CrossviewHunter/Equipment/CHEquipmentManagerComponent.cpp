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
	//기본 구조는 FLyraEquipmentList와 동일
	ULyraEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	

	TSubclassOf<ULyraEquipmentInstance> InstanceType = EquipmentDefinition->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = ULyraEquipmentInstance::StaticClass();
	}

	// 다운 캐스팅 -> CHEquipmentDef를 쓰기위함 (기존 Definition은 SubclassOf로 설정되어있어 값을 런타임중에 설정한 클래스를 사용하기 어려움)
	FCHAppliedEquipmentEntry& NewEntry = static_cast<FCHAppliedEquipmentEntry&>(Entries.AddDefaulted_GetRef());
	NewEntry.CHEquipmentDef = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;
	
	if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentDefinition->AbilitySetsToGrant)
		{
			if (AbilitySet != nullptr)
			{
				AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
			}
		}
		// Default 옵션 적용
		// GameplayEffectSpec기반으로 적용할 옵션을 생성
		TArray<FGameplayEffectSpecHandle> Handles = MakeGameEffectSpecHandles(ASC, EquipmentDefinition->GetDefaultOptions());
		for (FGameplayEffectSpecHandle Handle: Handles)
		{
			// ASC에 생성한 GE Spec 등록
			// FGameplayEffectSpecHandle ASC에서 GameplaySpec을 동적으로 생성할때 나오는 결과
			FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*Handle.Data);
			NewEntry.ActiveEffectHandles.Emplace(ActiveHandle);
		}
		// Additional 옵션 적용
		Handles.Empty();
		Handles = MakeGameEffectSpecHandles(ASC, EquipmentDefinition->GetAdditionalOptions());
		for (FGameplayEffectSpecHandle Handle: Handles)
		{
			FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*Handle.Data);
			NewEntry.ActiveAdditionalEffectHandles.Emplace(ActiveHandle);
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
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		//다운 캐스팅
		FCHAppliedEquipmentEntry& Entry = static_cast<FCHAppliedEquipmentEntry&>(*EntryIt);
		if (Entry.Instance == Instance)
		{
			if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
				for (const FActiveGameplayEffectHandle ActiveHandle : Entry.ActiveEffectHandles)
				{
					ASC->RemoveActiveGameplayEffect(ActiveHandle);
				}
				for (const FActiveGameplayEffectHandle ActiveHandle : Entry.ActiveAdditionalEffectHandles)
				{
					ASC->RemoveActiveGameplayEffect(ActiveHandle);
				}
				Entry.ActiveEffectHandles.Empty();
				Entry.ActiveAdditionalEffectHandles.Empty();
			}

			Instance->DestroyEquipmentActors();
			
			EntryIt.RemoveCurrent();
			MarkArrayDirty();

			break;
		}
	}
}

TArray<FGameplayEffectSpecHandle> FCHEquipmentList::MakeGameEffectSpecHandles(
	const TObjectPtr<ULyraAbilitySystemComponent>& ASC,
	TMap<TSubclassOf<UGameplayEffect>, int32> Modifiers) const
{
	TArray<FGameplayEffectSpecHandle> Results;
	for (auto It = Modifiers.CreateIterator(); It; ++It)
	{
		// Effect spec Handle 생성 (GE BP 클래스 기반)
		FGameplayEffectSpecHandle Handle = ASC->MakeOutgoingSpec(It.Key(), 1.0f, ASC->MakeEffectContext());
		// SetByCaller 설정을 위한 Tag 받아오기
		FGameplayTag Tag = Handle.Data->Def->Modifiers[0].ModifierMagnitude.GetSetByCallerFloat().DataTag;
		// SetByCaller에 값 설정
		Handle.Data->SetSetByCallerMagnitude(Tag, It.Value());

		Results.Add(Handle);
	}

	return Results;
}

TArray<FGameplayEffectSpecHandle> FCHEquipmentList::MakeGameEffectSpecHandles(
	const TObjectPtr<ULyraAbilitySystemComponent>& ASC,
	const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Modifiers) const
{
	TArray<FGameplayEffectSpecHandle> Results;
	for (TPair It : Modifiers)
	{
		// Effect spec Handle 생성 (GE BP 클래스 기반)
		FGameplayEffectSpecHandle Handle = ASC->MakeOutgoingSpec(It.Key, 1.0f, ASC->MakeEffectContext());
		// SetByCaller 설정을 위한 Tag 받아오기
		FGameplayTag Tag = Handle.Data->Def->Modifiers[0].ModifierMagnitude.GetSetByCallerFloat().DataTag;
		// SetByCaller에 값 설정
		Handle.Data->SetSetByCallerMagnitude(Tag, It.Value);

		Results.Add(Handle);
	}

	return Results;
	
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
		
		static_cast<FCHEquipmentList&>(EquipmentList).RemoveEntry(ItemInstance);
		
	}
}