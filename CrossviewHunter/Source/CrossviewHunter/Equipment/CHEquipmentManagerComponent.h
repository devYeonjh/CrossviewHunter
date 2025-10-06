// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Item/CHItemTypes.h"

#include "CHEquipmentManagerComponent.generated.h"


class UInventoryFragment_CHEquipmentInfo;
class UCHItemInstance;
class UCHEquipmentDefinition;
class UCHEquipmentManagerComponent;
class UGameplayEffect;
struct FCHEquipmentList;
struct FGameplayEffectSpecHandle;

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

	TArray<FGameplayEffectSpecHandle> MakeGameEffectSpecHandles(
		const TObjectPtr<ULyraAbilitySystemComponent>& ASC,
		TMap<TSubclassOf<UGameplayEffect>, float> Modifiers) const;

	// Additional 옵션을 위한 GameplayEffectSpec 생성.
	// TArray 사용 이유 : 정해진 크기의 배열을 받기위해
	// 중복된 옵션을 적용시키기 위해(TMap은 안됨(TMultiMap은 가능))
	// Index로 옵션을 관리하기 위해
	TArray<FGameplayEffectSpecHandle> MakeGameEffectSpecHandles(
		const TObjectPtr<ULyraAbilitySystemComponent>& ASC,
		const TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Modifiers) const;

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
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	ULyraEquipmentInstance* EquipItemInstance(UCHItemInstance* ItemInstance);

	virtual void UnequipItem(ULyraEquipmentInstance* ItemInstance) override;

};