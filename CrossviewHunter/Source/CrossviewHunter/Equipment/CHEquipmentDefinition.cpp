// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentDefinition.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystem/Attributes/CHStatEffectBase.h"

#include "Engine/GameInstance.h"


UCHEquipmentDefinition::UCHEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
}

void UCHEquipmentDefinition::SetDefaultOptions(const TMap<TSubclassOf<UGameplayEffect>, float>& Options)
{
	DefaultOptions = Options;
}

void UCHEquipmentDefinition::SetAdditionalOptions(const TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Options)
{
	AdditionalOptions = Options;
}

TMap<TSubclassOf<UGameplayEffect>, float> UCHEquipmentDefinition::GetDefaultOptions() const
{
	return DefaultOptions;
}

TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> UCHEquipmentDefinition::GetAdditionalOptions() const
{
	return AdditionalOptions;
}

