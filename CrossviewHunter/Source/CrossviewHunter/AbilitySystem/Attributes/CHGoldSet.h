// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"

#include "CHGoldSet.generated.h"

class UObject;
struct FFrame;

/**
 * UGoldSet
 *
 *	플레이어의 골드(화폐) 관련 스탯을 정의하는 클래스입니다.
 *	Attribute 예시: 골드
 */
UCLASS(Blueprintable, BlueprintType)
class CROSSVIEWHUNTER_API UCHGoldSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:

	UCHGoldSet();

	ATTRIBUTE_ACCESSORS(UCHGoldSet, Gold);

protected:
	
	UFUNCTION()
	void OnRep_Gold(const FGameplayAttributeData& OldValue);

private:
	
	// The current gold amount of the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Gold, Category = "CH|Gold", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Gold;
};