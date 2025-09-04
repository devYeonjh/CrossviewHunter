// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"

#include "PlayerStatusSet.generated.h"

class UObject;
struct FFrame;


/**
 * UPlayerStatusSet
 *
 *	플레이어의 전투 및 이동 관련 스탯을 정의하는 클래스입니다.
 *	Attribute 예시: 공격력, 방어력, 치명타율, 이동속도, 발사/재장전 딜레이 등
 */
UCLASS(BlueprintType)
class CROSSVIEWHUNTER_API UPlayerStatusSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:

	UPlayerStatusSet();

	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, Attack);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, Defence);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, CritRate);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, FireDelay);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, ReloadDelay);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, ArmorPiercingDamage);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, CritDamageRate);
	ATTRIBUTE_ACCESSORS(UPlayerStatusSet, HeadDamageRate);

protected:
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CritRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_FireDelay(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ReloadDelay(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ArmorPiercingDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CritDamageRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_HeadDamageRate(const FGameplayAttributeData& OldValue);


private:
	
	// The attack damage of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;

	// The defence rating of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defence, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defence;
	
	// The critical hit rate of the character (0.0 - 1.0).
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritRate, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritRate;

	// The movement speed of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	// The delay between firing shots in seconds.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireDelay, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireDelay;
	
	// The delay for reloading weapons in seconds.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReloadDelay, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ReloadDelay;
	
	// The armor piercing damage that ignores defense.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPiercingDamage, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ArmorPiercingDamage;

	// The critical damage multiplier when landing critical hits.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamageRate, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritDamageRate;

	// The damage multiplier when hitting the head.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HeadDamageRate, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HeadDamageRate;

	

};
