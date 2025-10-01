// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/CHStatSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"


class FLifetimeProperty;


UCHStatSet::UCHStatSet()
	: Attack(0.0f)
	, Defence(0.0f)
	, CritRate(0.0f)
	, MoveSpeed(0.0f)
	, FireDelay(0.0f)
	, ReloadDelay(0.0f)
	, ArmorPiercingDamage(0.0f)
	, CritDamage(0.0f)
	, HeadDamage(0.0f)
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
		ULyraAbilitySystemComponent* ASC = GetLyraAbilitySystemComponent();
		const ULyraHealthSet* HealthSet = Cast<ULyraHealthSet>(ASC->GetAttributeSet(ULyraHealthSet::StaticClass()));
		check(ASC);
		
		ASC->ApplyModToAttribute(HealthSet->GetMaxHealthAttribute(), EGameplayModOp::Override, GetHealth());
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
	else if (Attribute == GetCritRateAttribute())
	{
		OnCritRateChanged.Broadcast(GetCritRate());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		OnMoveSpeedChanged.Broadcast(GetMoveSpeed());
	}
	else if (Attribute == GetFireDelayAttribute())
	{
		OnFireDelayChanged.Broadcast(GetFireDelay());
	}
	else if (Attribute == GetReloadDelayAttribute())
	{
		OnReloadDelayChanged.Broadcast(GetReloadDelay());
	}
	else if (Attribute == GetArmorPiercingDamageAttribute())
	{
		OnArmorPiercingDamageChanged.Broadcast(GetArmorPiercingDamage());
	}
	else if (Attribute == GetCritDamageAttribute())
	{
		OnCritDamageChanged.Broadcast(GetCritDamage());
	}
	else if (Attribute == GetHeadDamageAttribute())
	{
		OnHeadDamageChanged.Broadcast(GetHeadDamage());
	}	

	
}


