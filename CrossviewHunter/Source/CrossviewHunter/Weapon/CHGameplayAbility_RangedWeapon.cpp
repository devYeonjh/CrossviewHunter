// Fill out your copyright notice in the Description page of Project Settings.


#include "CHGameplayAbility_RangedWeapon.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CHStatSet.h"

UCHGameplayAbility_RangedWeapon::UCHGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

float UCHGameplayAbility_RangedWeapon::GetFireDelayTimeSecs()
{
	float FinalFireDelay = FireDelayTimeSecs;
	bool bSuccess;
	float FireDelayRate = GetLyraAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValue(UCHStatSet::GetFireDelayAttribute(), bSuccess);
	if (bSuccess)
	{
		FinalFireDelay = FireDelayTimeSecs / FireDelayRate;
	}
	
	return FinalFireDelay;
}
