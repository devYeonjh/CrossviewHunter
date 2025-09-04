// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GoldSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;


UGoldSet::UGoldSet()
	: Gold(0.0f)
{
}

void UGoldSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGoldSet, Gold, COND_OwnerOnly, REPNOTIFY_Always);
}

void UGoldSet::OnRep_Gold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGoldSet, Gold, OldValue);
}