// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CHStatID.h"
#include "CHItemTypes.h"
#include "Equipment/LyraEquipmentDefinition.h"

#include "CHItemDataTableRows.generated.h"

#define MAX_ADDITIONAL_OPTION_COUNT 3

class ULyraAbilitySet;
class ULyraEquipmentInstance;
class ULyraInventoryItemDefinition;
struct FFrame;

UENUM(BlueprintType)
enum class ECHGradeID : uint8
{
	grade_20001 UMETA(DisplayName = "Common"),
	
	grade_20002 UMETA(DisplayName = "Rare"),
	
	grade_20003 UMETA(DisplayName = "Epic"),
	
	grade_20004 UMETA(DisplayName = "Unique"),
	
	grade_20005 UMETA(DisplayName = "Legendary"),
	
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECHOptionPoolID : uint8
{
	pool_20001 UMETA(DisplayName = "Weapon_Common_Options"),

	pool_20002 UMETA(DisplayName = "Armor_Common_Options"),

	pool_20003 UMETA(DisplayName = "Weapon_Unique_Options"),

	pool_20004 UMETA(DisplayName = "Armor_Unique_Options"),

	MAX UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FCHItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FCHItemDataTableRow()
	:	ItemID(FName())
	,	ItemName(FText())
	,	ItemType(ECHItemType::None)
	,	ItemGrade(ECHGradeID::grade_20001)
	,	EquipmentSlot(ECHEquipmentSlot::None)
	,	BaseStats(TMap<ECHStatID, float>())
	,	OptionPoolID(ECHOptionPoolID::pool_20001)
	,	SpecialEffectID(FName())
	{
		
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grade")
	ECHGradeID ItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHEquipmentSlot EquipmentSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TMap<ECHStatID, float> BaseStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHOptionPoolID OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FName SpecialEffectID;
};

USTRUCT(BlueprintType)
struct FCHItemOptionDetailRow : public FTableRowBase
{
	GENERATED_BODY()

	FCHItemOptionDetailRow()
	:	OptionID(FName())
	,	OptionPoolID(ECHOptionPoolID::pool_20001)
	,	StatID(ECHStatID::stat_001)
	,	MinValue(0)
	,	MaxValue(0)
	,	Weight(0)
	{
		
	}
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FName OptionID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHOptionPoolID OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHStatID StatID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 Weight;
	
};

USTRUCT(BlueprintType)
struct FCHEquipmentTypeDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	ECHItemType Type = ECHItemType::None;
	
	// Class to spawn
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSoftClassPtr<ULyraEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<TSoftObjectPtr<ULyraAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSoftClassPtr<ULyraInventoryItemDefinition> BaseItemDefinition;
};

USTRUCT(BlueprintType)
struct FCHItemGradeRow : public FTableRowBase
{
	GENERATED_BODY()

	FCHItemGradeRow()
	:	GradeID(ECHGradeID::grade_20001)
	,	AffixLines(0)
	,	ScrapMetal(0)
	,	Description(FString())
	{
		
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHGradeID GradeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	int32 AffixLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	int32 ScrapMetal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FString Description;
};


