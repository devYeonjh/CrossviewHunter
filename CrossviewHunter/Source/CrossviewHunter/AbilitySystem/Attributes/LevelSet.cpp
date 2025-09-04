// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/LevelSet.h"

#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;


ULevelSet::ULevelSet()
	: Level(1.0f)
	, EXP(0.0f)
{
}

void ULevelSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULevelSet, Level, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULevelSet, EXP, COND_OwnerOnly, REPNOTIFY_Always);
}

void ULevelSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelSet, Level, OldValue);
}

void ULevelSet::OnRep_EXP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelSet, EXP, OldValue);
}
