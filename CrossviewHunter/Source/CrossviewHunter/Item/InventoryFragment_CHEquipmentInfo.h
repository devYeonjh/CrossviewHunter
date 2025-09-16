// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/InventoryFragment_EquippableItem.h"
#include "AbilitySystem/CHStatID.h"
#include "CHItemTypes.h"
#include "Item/CHItemDataTableRows.h"

#include "InventoryFragment_CHEquipmentInfo.generated.h"

class UCHEquipmentDefinition;
class ULyraInventoryItemInstance;
class UObject;




/**
 * 장비의 종류, 슬롯, 랜덤 스탯 풀 정보 저장 (Data Table로 부터)
 * 스탯의 결정은 드롭할 때 결정
 */
UCLASS()
class CROSSVIEWHUNTER_API UInventoryFragment_CHEquipmentInfo : public UInventoryFragment_EquippableItem
{
	GENERATED_BODY()

	UInventoryFragment_CHEquipmentInfo();

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	ECHItemType Type;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	ECHEquipmentSlot Slot;
	
	UPROPERTY()
	TObjectPtr<UCHEquipmentDefinition> EquipmentDef;
	
public:
	UFUNCTION()
	void InitializeValue(ECHItemType InType, ECHEquipmentSlot InSlot, const TMap<ECHStatID, float>& InModifiers);

	UFUNCTION()
	void SetEquipmentDefinitionByData(const FEquipmentTypeDefinitionRow& DataRow) const;

	UFUNCTION()
	FORCEINLINE UCHEquipmentDefinition* GetEquipmentDef() const { return EquipmentDef;}
};
