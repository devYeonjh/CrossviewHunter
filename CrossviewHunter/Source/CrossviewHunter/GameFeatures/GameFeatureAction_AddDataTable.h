// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "Engine/DataTable.h"
#include "CrossviewHunter/Inventory/InventoryItemFragment_StatTable.h"
#include "GameFeatureAction_AddDataTable.generated.h"

USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FItemStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FString ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FString EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<FString, float> BaseStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FString PoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FString SpecialEffectID;
};

/**
 * Game Feature Action that manages item stat data table
 */
UCLASS(BlueprintType, meta = (DisplayName = "Add Data Table"))
class CROSSVIEWHUNTER_API UGameFeatureAction_AddDataTable : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	TMap<FString, TSoftObjectPtr<UDataTable>> ItemStatTables;

	UFUNCTION(BlueprintCallable, Category = "Item Stats")
	FItemStatTableRow GetItemStatByID(int32 ItemID, const FString& TableKey = TEXT("Default")) const;

	UFUNCTION(BlueprintCallable, Category = "Item Stats")
	bool FindItemStatByID(int32 ItemID, FItemStatTableRow& OutItemStat, const FString& TableKey = TEXT("Default")) const;
};
