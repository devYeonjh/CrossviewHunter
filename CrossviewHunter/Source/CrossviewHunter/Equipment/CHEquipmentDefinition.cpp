// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/CHAttributeHelpers.h"
#include "Engine/GameInstance.h"
#include "AbilitySystem/CHStatID.h"
#include "Kismet/GameplayStatics.h"

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

void UCHEquipmentDefinition::ApplyModifiers(ULyraAbilitySystemComponent* ASC)
{
	if (Modifiers.Num() == 0)
		return;
	

	UGameplayEffect* RuntimeGE = NewObject<UGameplayEffect>(GetTransientPackage(),
		FName("RuntimeGE_" + GetNameSafe(this) + FString::FromInt(EquipmentGENameOffsetNumber)));

	RuntimeGE->DurationPolicy = EGameplayEffectDurationType::Infinite;
	EquipmentGENameOffsetNumber++;

	int32 Idx = RuntimeGE->Modifiers.Num();
	RuntimeGE->Modifiers.SetNum(Idx + Modifiers.Num());

	int32 tempIdxDisplacement = 0;
	
	UCHAttributeHelpers* AttributeHelpers = UGameplayStatics::GetGameInstance(ASC->GetOwner()->GetWorld())->GetSubsystem<UCHAttributeHelpers>();
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

}

void UCHEquipmentDefinition::RemoveModifiers(ULyraAbilitySystemComponent* ASC)
{
	if (RuntimeGEHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(RuntimeGEHandle);
		RuntimeGEHandle.Invalidate();
	}
}


