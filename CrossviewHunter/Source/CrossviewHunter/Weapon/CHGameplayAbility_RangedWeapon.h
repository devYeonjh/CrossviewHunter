// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/LyraGameplayAbility_RangedWeapon.h"
#include "CHGameplayAbility_RangedWeapon.generated.h"

/**
 * 연사 속도 설정 추가
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHGameplayAbility_RangedWeapon : public ULyraGameplayAbility_RangedWeapon
{
	GENERATED_BODY()

	UCHGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UFUNCTION(BlueprintCallable)
	float GetFireDelayTimeSecs();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FireDelayTimeSecs = 1.0f;
};
