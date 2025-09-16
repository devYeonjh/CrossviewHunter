// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/CHAttributeHelpers.h"
#include "Engine/GameInstance.h"
#include "AbilitySystem/CHStatID.h"
#include "Character/LyraCharacter.h"

UCHEquipmentDefinition::UCHEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
	EquipmentGENameOffsetNumber = 0;
}

void UCHEquipmentDefinition::SetModifiers(const TMap<ECHStatID, float>& InModifiers)
{
	Modifiers = InModifiers;
}

void UCHEquipmentDefinition::ApplyModifiers(const ALyraCharacter* Owner)
{
	if (Modifiers.Num() == 0)
		return;
	if (ULyraAbilitySystemComponent* ASC = Owner->GetLyraAbilitySystemComponent() )
	{
		UGameplayEffect* RuntimeGE = NewObject<UGameplayEffect>(GetTransientPackage(),
			FName("RuntimeGE_" + GetNameSafe(this) + FString::FromInt(EquipmentGENameOffsetNumber)));

		RuntimeGE->DurationPolicy = EGameplayEffectDurationType::Infinite;
		EquipmentGENameOffsetNumber++;

		int32 Idx = RuntimeGE->Modifiers.Num();
		RuntimeGE->Modifiers.SetNum(Idx + Modifiers.Num());

		int32 tempIdxDisplacement = 0;
		
		UWorld* World = Owner->GetWorld();
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
	
		RuntimeGEHandle = ASC->ApplyGameplayEffectToSelf(RuntimeGE, 1.0f, ASC->MakeEffectContext());
		UE_LOG(LogTemp, Log, TEXT("RuntimeGEHandle: %s"), *RuntimeGEHandle.ToString());
		UE_LOG(LogTemp, Log, TEXT("RuntimeGE: %s"), *RuntimeGE->GetName());
	}
}

void UCHEquipmentDefinition::RemoveModifiers()
{
	if (RuntimeGEHandle.IsValid())
	{
		RuntimeGEHandle.RemoveFromGlobalMap();
	}
}


