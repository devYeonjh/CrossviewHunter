// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CHItemTypes.generated.h"


UENUM()
enum class ECHItemType : uint8
{
	None,
	Pistol,
	Rifle,
	Shotgun,
	GrenadeLauncher,
	Bow,
	Flamethrower,
	Core,
	
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

