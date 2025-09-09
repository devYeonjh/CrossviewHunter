// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/LyraInventoryItemDefinition.h"
#include "InventoryFragment_Stackable.generated.h"

UCLASS()
class LYRAGAME_API UInventoryFragment_Stackable : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	// The maximum number of this item that can be stacked.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lyra|Inventory")
	int32 MaxStackSize = 9999;
};