// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentDefinition.h"

#include "CHEquipmentDefinition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Const, BlueprintType)
class CROSSVIEWHUNTER_API UCHEquipmentDefinition : public ULyraEquipmentDefinition
{
	GENERATED_BODY()

public:
	UCHEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};
