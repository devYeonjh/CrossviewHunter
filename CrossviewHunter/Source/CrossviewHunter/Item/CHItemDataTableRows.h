// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CHStatID.h"
#include "CHItemTypes.h"
#include "ScalableFloat.h"
#include "Equipment/LyraEquipmentDefinition.h"

#include "CHItemDataTableRows.generated.h"

class ULyraAbilitySet;
class ULyraEquipmentInstance;

USTRUCT(BlueprintType)
struct FItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHItemType ItemType = ECHItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHEquipmentSlot EquipmentSlot = ECHEquipmentSlot::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TMap<ECHStatID, float> BaseStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FName OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FName SpecialEffectID;
};

USTRUCT(BlueprintType)
struct FItemOptionListRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FName OptionID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FName OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHStatID StatID = ECHStatID::stat_001;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FScalableFloat Value;
	
};

USTRUCT(BlueprintType)
struct FEquipmentTypeDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	ECHItemType Type = ECHItemType::None;
	
	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSoftClassPtr<ULyraEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TSoftObjectPtr<ULyraAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;
};