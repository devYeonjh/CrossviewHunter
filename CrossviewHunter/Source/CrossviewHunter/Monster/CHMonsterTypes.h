// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CHMonsterTypes.generated.h"

UENUM()
enum class ECHMonsterGrade : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Boss UMETA(DisplayName = "Boss"),

	MAX UMETA(Hidden)
};