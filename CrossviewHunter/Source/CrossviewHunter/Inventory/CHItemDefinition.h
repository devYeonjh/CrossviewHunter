// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "AbilitySystem/CHAttributeTypes.h"

#include "CHItemDefinition.generated.h"



UENUM()
enum class ECHItemType : uint8
{
	Equipment UMETA(DisplayName = "Equipment"),
	
	Consumable UMETA(DisplayName = "Consumable"),
	
	MAX UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
    ECHItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display,
		meta=(AllowedClasses="LyraInventoryItemFragment", ShowOnlyInnerProperties))
	TArray<TObjectPtr<ULyraInventoryItemFragment>>  Fragments;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    FName PoolID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    FName SpecialEffectID;
};


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
