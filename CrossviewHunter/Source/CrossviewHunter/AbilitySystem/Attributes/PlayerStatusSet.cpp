// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/PlayerStatusSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;


UPlayerStatusSet::UPlayerStatusSet()
	: Attack(10.0f)
	, Defence(5.0f)
	, CritRate(0.05f)
	, MoveSpeed(600.0f)
	, FireDelay(0.5f)
	, ReloadDelay(2.0f)
	, ArmorPiercingDamage(0.0f)
	, CritDamageRate(1.5f)
	, HeadDamageRate(2.0f)
{
}

void UPlayerStatusSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, Attack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, Defence, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, CritRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, MoveSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, FireDelay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, ReloadDelay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, ArmorPiercingDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, CritDamageRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerStatusSet, HeadDamageRate, COND_OwnerOnly, REPNOTIFY_Always);
}

void UPlayerStatusSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, Attack, OldValue);
}

void UPlayerStatusSet::OnRep_Defence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, Defence, OldValue);
}

void UPlayerStatusSet::OnRep_CritRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, CritRate, OldValue);
}

void UPlayerStatusSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, MoveSpeed, OldValue);
}

void UPlayerStatusSet::OnRep_FireDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, FireDelay, OldValue);
}

void UPlayerStatusSet::OnRep_ReloadDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, ReloadDelay, OldValue);
}

void UPlayerStatusSet::OnRep_ArmorPiercingDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, ArmorPiercingDamage, OldValue);
}

void UPlayerStatusSet::OnRep_CritDamageRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, CritDamageRate, OldValue);
}

void UPlayerStatusSet::OnRep_HeadDamageRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerStatusSet, HeadDamageRate, OldValue);
}