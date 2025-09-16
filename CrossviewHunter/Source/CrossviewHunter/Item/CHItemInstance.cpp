// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemInstance.h"
#include "Inventory/LyraInventoryItemDefinition.h"


void UCHItemInstance::SetItemDefinitionData(TObjectPtr<ULyraInventoryItemDefinition> ItemDefinition)
{
	SetItemDefInstance(ItemDefinition);
}
