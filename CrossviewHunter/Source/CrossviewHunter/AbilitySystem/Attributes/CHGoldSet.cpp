// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/CHGoldSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;


UCHGoldSet::UCHGoldSet()
	: Gold(0.0f)
{
}

void UCHGoldSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCHGoldSet, Gold, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCHGoldSet::OnRep_Gold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHGoldSet, Gold, OldValue);
}