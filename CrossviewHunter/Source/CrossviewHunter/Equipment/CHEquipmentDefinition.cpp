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

void UCHEquipmentDefinition::SetDefaultOptions(const TMap<TSubclassOf<UGameplayEffect>, int32>& Options)
{
	DefaultOptions = Options;
}

void UCHEquipmentDefinition::SetAdditionalOptions(const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Options)
{
	AdditionalOptions = Options;
}

void UCHEquipmentDefinition::SetOptionDetails(const TArray<FCHItemOptionDetailRow>& Options)
{
	OptionDetails = Options;
}

TMap<TSubclassOf<UGameplayEffect>, int32> UCHEquipmentDefinition::GetDefaultOptions() const
{
	return DefaultOptions;
}

TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>
UCHEquipmentDefinition::GetAdditionalOptions() const
{
	return AdditionalOptions;
}

TArray<FCHItemOptionDetailRow> UCHEquipmentDefinition::GetOptionDetails() const
{
	return OptionDetails;
}

