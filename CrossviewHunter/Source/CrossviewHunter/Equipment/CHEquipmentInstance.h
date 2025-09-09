// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/LyraCharacter.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "AbilitySystem/CHAttributeTypes.h"

#include "CHEquipmentInstance.generated.h"

class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHEquipmentInstance : public ULyraEquipmentInstance
{
	GENERATED_BODY()

public:
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

	UFUNCTION(BlueprintCallable, Category = "CH|Equipment")
	void OnEquipped(ALyraCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "CH|Equipment")
	void SetOwner(ALyraCharacter* Character);

	void SetModifiers(const TMap<ECHStatID, float>& InModifiers);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CH|Equipment")
	TMap<ECHStatID, float> Modifiers;

private:
	FActiveGameplayEffectHandle RuntimeGEHandle;
	
	void CreateAndApplyModifierEffects();
	
	TWeakObjectPtr<ALyraCharacter> OwningCharacter;
	int EquipmentGENameOffsetNumber;
	
	
};
