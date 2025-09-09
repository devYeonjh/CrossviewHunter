// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CHAttributeTypes.generated.h"

UENUM(BlueprintType)
enum class ECHStatID : uint8
{
	stat_001 UMETA(DisplayName = "Health"),
	
	stat_002 UMETA(DisplayName = "Attack"),

	stat_003 UMETA(DisplayName = "Defence"),

	stat_004 UMETA(DisplayName = "CritRate"),

	stat_005 UMETA(DisplayName = "MoveSpeed"),

	stat_006 UMETA(DisplayName = "FireDelay"),

	stat_007 UMETA(DisplayName = "ReloadDelay"),

	stat_008 UMETA(DisplayName = "ArmorPiercingDamage"),

	stat_009 UMETA(DisplayName = "CritDamage"),
	
	stat_010 UMETA(DisplayName = "HeadDamage"),

	MAX UMETA(Hidden)
};

