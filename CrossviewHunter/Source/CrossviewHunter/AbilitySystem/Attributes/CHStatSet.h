// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystem/CHAttributeTypes.h"

#include "CHStatSet.generated.h"

class UObject;
struct FFrame;

class UDataTable;

DECLARE_MULTICAST_DELEGATE_OneParam(FCHAttributeChange, float /*NewValue*/);


USTRUCT(BlueprintType)
struct FCHStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CH|StatData")
	ECHStatID AttributeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CH|StatData")
	FGameplayAttribute AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CH|StatData")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CH|StatData")
	bool bIsPercentage;
};


/**
 * UPlayerStatusSet
 *
 *	플레이어의 전투 및 이동 관련 스탯을 정의하는 클래스입니다.
 */
UCLASS(BlueprintType)
class CROSSVIEWHUNTER_API UCHStatSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:

	UCHStatSet();

	ATTRIBUTE_ACCESSORS(UCHStatSet, Health);
	ATTRIBUTE_ACCESSORS(UCHStatSet, Attack);
	ATTRIBUTE_ACCESSORS(UCHStatSet, Defence);
	ATTRIBUTE_ACCESSORS(UCHStatSet, CritRate);
	ATTRIBUTE_ACCESSORS(UCHStatSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UCHStatSet, FireDelay);
	ATTRIBUTE_ACCESSORS(UCHStatSet, ReloadDelay);
	ATTRIBUTE_ACCESSORS(UCHStatSet, ArmorPiercingDamage);
	ATTRIBUTE_ACCESSORS(UCHStatSet, CritDamage);
	ATTRIBUTE_ACCESSORS(UCHStatSet, HeadDamage);

	mutable FCHAttributeChange OnHealthChanged;

	mutable FCHAttributeChange OnAttackChanged;

	mutable FCHAttributeChange OnDefenceChanged;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	
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
	void OnRep_CritDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_HeadDamage(const FGameplayAttributeData& OldValue);

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;




private:
	// The max health of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
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
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritDamage;

	// The damage multiplier when hitting the head.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HeadDamage, Category = "CH|PlayerStatus", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HeadDamage;
};
