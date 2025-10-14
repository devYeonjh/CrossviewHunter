// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CHItemDataTableRows.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "CHItemTypes.h"

#include "InventoryFragment_CHEquipmentInfo.generated.h"

struct FCHEquipmentTypeDefinitionRow;
class UCHEquipmentDefinition;
class ULyraInventoryItemInstance;
class UObject;
class UGameplayEffect;




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
	UPROPERTY()
	TObjectPtr<UCHEquipmentDefinition> EquipmentDef;

	UPROPERTY()
	FCHItemDataTableRow ItemData;
	
public:
	void InitializeValue(
		const FCHItemDataTableRow& ItemDataRow,
		const TMap<TSubclassOf<UGameplayEffect>, int32>& DefaultOptions,
		const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& AdditionalOptions,
		const TArray<FCHItemOptionDetailRow>& OptionDetails,
		int32 DecomposeItemCount);

	void ChangeAdditionalOption(const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& AdditionalOptions) const;

	/** WID에 있는 AbilitySet과 ActorToSpawn 데이터 설정 */
	UFUNCTION()
	void SetEquipmentDefinitionByData(const FCHEquipmentTypeDefinitionRow& DataRow) const;

	UFUNCTION(BlueprintCallable, Category = "CH|InventoryFragment_CHEquipmentInfo")
	FORCEINLINE UCHEquipmentDefinition* GetEquipmentDef() const { return EquipmentDef;}

	UFUNCTION(BlueprintCallable, Category = "CH|InventoryFragment_CHEquipmentInfo")
	FORCEINLINE FCHItemDataTableRow GetItemData() const {return ItemData;} 
};
