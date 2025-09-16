// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include "CHEquipmentManagerComponent.generated.h"


class UInventoryFragment_CHEquipmentInfo;
class UCHItemInstance;
class UCHEquipmentDefinition;
class UCHEquipmentManagerComponent;
struct FCHEquipmentList;

USTRUCT(BlueprintType)
struct FCHAppliedEquipmentEntry : public FLyraAppliedEquipmentEntry
{
	GENERATED_BODY()

private:
	friend FCHEquipmentList;
	friend UCHEquipmentManagerComponent;

	// The CHEquipmentDefinition
	UPROPERTY()
	TObjectPtr<UCHEquipmentDefinition> CHEquipmentDef;
};

/** Base on FLyraEquipmentList */
USTRUCT(BlueprintType)
struct FCHEquipmentList : public FLyraEquipmentList
{
	GENERATED_BODY()

public:
	ULyraEquipmentInstance* AddEntry(UCHEquipmentDefinition* EquipmentDefinition);
	void RemoveEntry(ULyraEquipmentInstance* Instance);

protected:
	friend UCHEquipmentManagerComponent;
	
};



/**
 * 
 * 
 */
UCLASS(BlueprintType, Const)
class CROSSVIEWHUNTER_API UCHEquipmentManagerComponent : public ULyraEquipmentManagerComponent
{
	GENERATED_BODY()

public:
	ULyraEquipmentInstance* EquipItem(UCHItemInstance* ItemInstance);

	virtual void UnequipItem(ULyraEquipmentInstance* ItemInstance) override;

};