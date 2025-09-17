// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CHStatEffectBase.h"

#include "Engine/GameInstance.h"


UCHEquipmentDefinition::UCHEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
}

void UCHEquipmentDefinition::SetModifiers(const TMap<TSubclassOf<UGameplayEffect>, float>& InModifiers)
{
	Modifiers = InModifiers;
}

TMap<TSubclassOf<UGameplayEffect>, float> UCHEquipmentDefinition::GetModifiers() const
{
	return Modifiers;
}

