// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"

void UCHItemDefinition::SetFragments(const TArray<TObjectPtr<ULyraInventoryItemFragment>>& InFragmentClasses)
{
	Fragments = InFragmentClasses;
}

void UCHItemDefinition::SetModifiers(const TMap<ECHStatID, float>& InModifiers)
{
	Modifiers = InModifiers;
}
