// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/LyraEquipmentDefinition.h"
#include "ActiveGameplayEffectHandle.h"

#include "CHEquipmentDefinition.generated.h"

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

	void SetModifiers(const TMap<ECHStatID, float>& InModifiers);
	
	void ApplyModifiers(const ALyraCharacter* Owner);

	void RemoveModifiers();

private:
	UPROPERTY()
	FActiveGameplayEffectHandle RuntimeGEHandle;
	int EquipmentGENameOffsetNumber;

	TMap<ECHStatID, float> Modifiers;
};
