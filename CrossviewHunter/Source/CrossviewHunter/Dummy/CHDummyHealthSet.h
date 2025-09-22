// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "CHDummyHealthSet.generated.h"

/**
 * Dummy Health Set
 *
 * This attribute set will not allow the Target Dummy to go below 1 Health.
 */
UCLASS(BlueprintType)
class CROSSVIEWHUNTER_API UCHDummyHealthSet : public ULyraHealthSet
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	UCHDummyHealthSet();

	//~ULyraHealthSet implementation
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//~End of ULyraHealthSet implementation
};
