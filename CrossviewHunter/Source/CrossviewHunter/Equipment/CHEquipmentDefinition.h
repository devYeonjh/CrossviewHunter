// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentDefinition.h"
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

	void SetDefaultOptions(const TMap<TSubclassOf<UGameplayEffect>, float>& Options);
	void SetAdditionalOptions(const TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>& Options);

	TMap<TSubclassOf<UGameplayEffect>, float>GetDefaultOptions() const;
	TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>GetAdditionalOptions() const;

private:

	TMap<TSubclassOf<UGameplayEffect>, float> DefaultOptions;
	TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>>AdditionalOptions;
};
