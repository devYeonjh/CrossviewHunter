// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentManagerComponent.h"
#include "CHEquipmentManagerComponent.generated.h"


class UCHEquipmentDefinition;
class UCHEquipmentInstance;
class UCHEquipmentManagerComponent;
struct FCHEquipmentList;

/** Base on FLyraAppliedEquipmentEntry */
USTRUCT(BlueprintType)
struct FCHAppliedEquipmentEntry : public FLyraAppliedEquipmentEntry
{
	GENERATED_BODY()

	FCHAppliedEquipmentEntry()
	{}
	
};

/** Base on FLyraEquipmentList */
USTRUCT(BlueprintType)
struct FCHEquipmentList : public FLyraEquipmentList
{
	GENERATED_BODY()

public:
	UCHEquipmentInstance* AddEntry(TSubclassOf<UCHEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UCHEquipmentInstance* Instance);

private:
	friend UCHEquipmentManagerComponent;
	
};



/**
 * Pawn Component로 부착되는 Equipment 매니저 클래스
 * 
 */
UCLASS(BlueprintType, Const)
class CROSSVIEWHUNTER_API UCHEquipmentManagerComponent : public ULyraEquipmentManagerComponent
{
	GENERATED_BODY()

};
