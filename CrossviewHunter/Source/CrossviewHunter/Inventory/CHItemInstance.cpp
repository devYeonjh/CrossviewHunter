// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemInstance.h"
#include "CHItemDefinition.h"

bool UCHItemInstance::SetItemDefinitionData(
	const TArray<TObjectPtr<ULyraInventoryItemFragment>>& FragmentClasses,
	const TMap<ECHStatID, float>& Modifiers )
{
	if (UCHItemDefinition* CHItemDef = Cast<UCHItemDefinition>(ItemDef))
	{
		CHItemDef->SetFragments(FragmentClasses);
		CHItemDef->SetModifiers(Modifiers);

		return true;
	}
	
	return false;
}
