// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/LyraInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquippableItem.generated.h"

#define UE_API LYRAGAME_API

class ULyraEquipmentDefinition;
class UObject;

UCLASS()
class UE_API UInventoryFragment_EquippableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Lyra)
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lyra|Inventory")
	TSubclassOf<ULyraInventoryItemDefinition> DecomposeItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lyra|Inventory")
	int32 DecomposeItemAmount = 10;
};

#undef UE_API