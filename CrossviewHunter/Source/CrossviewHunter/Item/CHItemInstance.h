// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include "CHItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHItemInstance : public ULyraInventoryItemInstance
{
	GENERATED_BODY()

public:
	void SetItemDefinitionData(TObjectPtr<ULyraInventoryItemDefinition> ItemDefinition);
	
};
