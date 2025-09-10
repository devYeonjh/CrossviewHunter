// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/LyraInventoryItemDefinition.h"
#include "AbilitySystem/CHAttributeTypes.h"

#include "InventoryFragment_CHEquipmentInfo.generated.h"

class ULyraInventoryItemInstance;
class UObject;

UENUM()
enum class ECHEquipmentType : uint8
{
	Pistol UMETA(DisplayName="Pistol"),
	
	Rifle UMETA(DisplayName="Rifle"),
	
	Shotgun UMETA(DisplayName="Shotgun"),
	
	GrenadeLauncher UMETA(DisplayName="Grenade Launcher"),
	
	Bow UMETA(DisplayName="Bow"),
	
	Flamethrower UMETA(DisplayName="Flamethrower"),
	
	Core UMETA(DisplayName="Core"),
	
	MAX UMETA(Hidden)
};

UENUM()
enum class ECHEquipmentSlot : uint8
{
	Weapon UMETA(DisplayName="Weapon"),

	Core UMETA(DisplayName="Core"),

	MAX UMETA(Hidden)
};


/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UInventoryFragment_CHEquipmentInfo : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	ECHEquipmentType Type;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	ECHEquipmentSlot Slot;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<ECHStatID, float> Modifiers;

public:
	UFUNCTION()
	void SetModifiers(const TMap<ECHStatID, float>& InModifiers);

	UFUNCTION()
	TMap<ECHStatID, float> GetModifiers() const;
};
