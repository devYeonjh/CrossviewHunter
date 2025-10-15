// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentDefinition.h"
#include "Item/CHItemDataTableRows.h"
#include "Item/CHItemTypes.h"

#include "CHEquipmentDefinition.generated.h"

class UGameplayEffect;
class ULyraAbilitySystemComponent;
enum class ECHStatID : uint8;
class UObject;
class ALyraCharacter;
struct FFrame;

/**
 * 장착 장비의 스탯 적용 정보
 */
UCLASS(Blueprintable, Const, BlueprintType)
class CROSSVIEWHUNTER_API UCHEquipmentDefinition : public ULyraEquipmentDefinition
{
	GENERATED_BODY()

public:
	UCHEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetDefaultOptions(const TMap<TSubclassOf<UGameplayEffect>, int32>& Options);
	void SetAdditionalOptions(const TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Options);
	void SetOptionDetails(const TArray<FCHItemOptionDetailRow>& Options);

	TMap<TSubclassOf<UGameplayEffect>, int32> GetDefaultOptions() const;
	TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> GetAdditionalOptions() const;
	TArray<FCHItemOptionDetailRow> GetOptionDetails() const;

private:
	TMap<TSubclassOf<UGameplayEffect>, int32> DefaultOptions;
	TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions;
	TArray<FCHItemOptionDetailRow> OptionDetails;
};
