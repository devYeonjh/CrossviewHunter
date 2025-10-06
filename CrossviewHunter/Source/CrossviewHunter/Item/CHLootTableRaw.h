// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHItemTypes.h"
#include "GameplayTagContainer.h"

#include "CHLootTableRaw.generated.h"


USTRUCT(BlueprintType)
struct FCHLootGroupRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable", meta = (Categories = "CH.DT.LootGroup.ID"))
	FGameplayTag LootGroupID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	int32 QuantityMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	int32 QuantityMin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	ECHEquipmentSlot RewardType;
};

USTRUCT(BlueprintType)
struct FCHLootTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable", meta = (Categories = "CH.DT.LootTable.ID"))
	FGameplayTag LootTableID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable", meta = (Categories = "CH.DT.LootGroup.ID"))
	FGameplayTag LootGroupID;
};