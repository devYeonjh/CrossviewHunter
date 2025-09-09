// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "AbilitySystem/CHAttributeTypes.h"

#include "CHItemDefinition.generated.h"

USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FItemDataTableRow : public FTableRowBase
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


UENUM()
enum class EItemType
{
	Equipment UMETA(DisplayName = "Equipment"),
	Consumable UMETA(DisplayName = "Consumable"),
	MAX UMETA(Hidden)
};


/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHItemDefinition : public ULyraInventoryItemDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TMap<ECHStatID, float> Modifiers;

public:
	void SetFragments(const TArray<TObjectPtr<ULyraInventoryItemFragment>>& InFragmentClasses);
	void SetModifiers(const TMap<ECHStatID, float>& InModifiers);

	

};
