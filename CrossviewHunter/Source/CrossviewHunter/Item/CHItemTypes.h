// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CHItemTypes.generated.h"

#define MAX_ADDITIONAL_OPTION_COUNT 3

UENUM()
enum class ECHItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),
	Bow UMETA(DisplayName = "Bow"),
	Flamethrower UMETA(DisplayName = "Flamethrower"),
	Core UMETA(DisplayName = "Core"),
	
	MAX UMETA(Hidden)
};

UENUM()
enum class ECHEquipmentSlot : uint8
{
	None,
	Weapon,
	Core,

	MAX UMETA(Hidden)
};

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