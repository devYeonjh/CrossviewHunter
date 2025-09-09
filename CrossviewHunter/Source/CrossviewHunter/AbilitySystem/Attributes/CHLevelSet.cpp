// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/CHLevelSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;


UCHLevelSet::UCHLevelSet()
	: Level(1.0f)
	, EXP(0.0f)
{
}

void UCHLevelSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCHLevelSet, Level, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCHLevelSet, EXP, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCHLevelSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHLevelSet, Level, OldValue);
}

void UCHLevelSet::OnRep_EXP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCHLevelSet, EXP, OldValue);
}
