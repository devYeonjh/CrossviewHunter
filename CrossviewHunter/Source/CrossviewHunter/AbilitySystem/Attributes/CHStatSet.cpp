// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/CHStatSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


class FLifetimeProperty;


UCHStatSet::UCHStatSet()
	: Attack(10.0f)
	, Defence(5.0f)
	, CritRate(0.05f)
	, MoveSpeed(600.0f)
	, FireDelay(0.5f)
	, ReloadDelay(2.0f)
	, ArmorPiercingDamage(0.0f)
	, CritDamage(1.5f)
	, HeadDamage(2.0f)
{
}

void UCHStatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, Health, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, Attack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, Defence, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, CritRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, MoveSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, FireDelay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, ReloadDelay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, ArmorPiercingDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, CritDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHStatSet, HeadDamage, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCHStatSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, Health, OldValue);
}

void UCHStatSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, Attack, OldValue);
}

void UCHStatSet::OnRep_Defence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, Defence, OldValue);
}

void UCHStatSet::OnRep_CritRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, CritRate, OldValue);
}

void UCHStatSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, MoveSpeed, OldValue);
}

void UCHStatSet::OnRep_FireDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, FireDelay, OldValue);
}

void UCHStatSet::OnRep_ReloadDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, ReloadDelay, OldValue);
}

void UCHStatSet::OnRep_ArmorPiercingDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, ArmorPiercingDamage, OldValue);
}

void UCHStatSet::OnRep_CritDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, CritDamage, OldValue);
}

void UCHStatSet::OnRep_HeadDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHStatSet, HeadDamage, OldValue);
}

void UCHStatSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(GetHealth());
	}
	else if (Attribute == GetAttackAttribute())
	{
		OnAttackChanged.Broadcast(GetAttack());
	}
	else if (Attribute == GetDefenceAttribute())
	{
		OnDefenceChanged.Broadcast(GetDefence());
	}
}


