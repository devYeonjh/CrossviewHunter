// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CHLootTableRow.generated.h"


USTRUCT(BlueprintType)
struct FCHLootGroupEntryRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FName LootGroupEntryID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FName LootGroupID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable", meta = (ClampMin = 0))
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
	FName RewardType;
};

USTRUCT(BlueprintType)
struct FCHLootTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FName LootTableID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	float DropChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTable")
	FName LootGroupID;
};