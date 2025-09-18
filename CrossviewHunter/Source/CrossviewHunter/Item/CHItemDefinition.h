// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "CHItemDataTableRows.h"

#include "CHItemDefinition.generated.h"

class UCHItemCreationComponent;

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHItemDefinition : public ULyraInventoryItemDefinition
{
	GENERATED_BODY()

public:
	void SetItemData(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow);

	void SetEquipmentFragment(UCHItemCreationComponent* ICC, const FCHItemDataTableRow& DataTableRow);

};
