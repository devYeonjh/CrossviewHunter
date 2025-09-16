// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "CHItemDataTableRows.h"

#include "CHItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHItemDefinition : public ULyraInventoryItemDefinition
{
	GENERATED_BODY()

public:
	void SetItemData(const FItemDataTableRow& DataTableRow);
};
