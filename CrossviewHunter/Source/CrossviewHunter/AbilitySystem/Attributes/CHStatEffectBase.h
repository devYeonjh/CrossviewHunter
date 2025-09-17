// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CHStatEffectBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class CROSSVIEWHUNTER_API UCHStatEffectBase : public UGameplayEffect
{
	GENERATED_BODY()

	UCHStatEffectBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
