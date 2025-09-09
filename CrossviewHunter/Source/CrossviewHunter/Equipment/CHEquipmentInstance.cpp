// Fill out your copyright notice in the Description page of Project Settings.

#include "CHEquipmentInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystem/CHAttributeHelpers.h"
#include "Engine/GameInstance.h"


void UCHEquipmentInstance::OnEquipped()
{
	Super::OnEquipped();
	EquipmentGENameOffsetNumber = 0;
	CreateAndApplyModifierEffects();
	
}

void UCHEquipmentInstance::OnUnequipped()
{
	Super::OnUnequipped();
}

void UCHEquipmentInstance::OnEquipped(ALyraCharacter* Character)
{
	OwningCharacter = Character;
	OnEquipped();
}

void UCHEquipmentInstance::SetOwner(ALyraCharacter* Character)
{
	OwningCharacter = Character;
}

void UCHEquipmentInstance::SetModifiers(const TMap<ECHStatID, float>& InModifiers)
{
	Modifiers = InModifiers;
}

void UCHEquipmentInstance::CreateAndApplyModifierEffects()
{
	if (ULyraAbilitySystemComponent* LASC = OwningCharacter->GetLyraAbilitySystemComponent() )
	{
		UGameplayEffect* RuntimeGE = NewObject<UGameplayEffect>(GetTransientPackage(),
			FName("RuntimeGE_" + GetNameSafe(this) + FString::FromInt(EquipmentGENameOffsetNumber)));

		RuntimeGE->DurationPolicy = EGameplayEffectDurationType::Infinite;
		EquipmentGENameOffsetNumber++;

		int32 Idx = RuntimeGE->Modifiers.Num();
		RuntimeGE->Modifiers.SetNum(Idx + Modifiers.Num());

		int32 tempIdxDisplacement = 0;
		
		UWorld* World = OwningCharacter->GetWorld();
		if (World == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("World is null"));
			return;
		}

		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GameInstance is null"));
			return;
		}

		UCHAttributeHelpers* AttributeHelpers = GameInstance->GetSubsystem<UCHAttributeHelpers>();
		if (AttributeHelpers == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AttributeHelpers is null"));
			return;
		}

		for (const TPair<ECHStatID, float>& Modify : Modifiers)
		{
			FGameplayTagRequirements reqs;
			reqs.IgnoreTags = FGameplayTagContainer();
			reqs.RequireTags = FGameplayTagContainer();
			FGameplayAttribute Attribute;
			AttributeHelpers->GetAttributeForID(Modify.Key, Attribute);
	
			FGameplayModifierInfo& Info = RuntimeGE->Modifiers[Idx + tempIdxDisplacement];
			Info.ModifierMagnitude = FScalableFloat(Modify.Value);
			Info.ModifierOp = EGameplayModOp::Additive;
			Info.Attribute = Attribute;
			Info.SourceTags = reqs;
			Info.TargetTags = reqs;
	
			tempIdxDisplacement++;
		}
	
		RuntimeGEHandle = LASC->ApplyGameplayEffectToSelf(RuntimeGE, 1.0f, LASC->MakeEffectContext());
		UE_LOG(LogTemp, Log, TEXT("RuntimeGEHandle: %s"), *RuntimeGEHandle.ToString());
		UE_LOG(LogTemp, Log, TEXT("RuntimeGE: %s"), *RuntimeGE->GetName());
	}
}
