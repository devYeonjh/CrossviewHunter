// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "CHAbilitySystemComponent.generated.h"


UCLASS()
class CROSSVIEWHUNTER_API UCHAbilitySystemComponent : public ULyraAbilitySystemComponent
{
	GENERATED_BODY()

	UCHAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
