// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"

#include "CHLevelSet.generated.h"

class UObject;
struct FFrame;

/**
 * ULevelSet
 *
 *	레벨 및 경험치 관련 스탯을 정의하는 클래스입니다.
 *	Attribute 예시: 레벨, 경험치
 */
UCLASS(Blueprintable, BlueprintType)
class CROSSVIEWHUNTER_API UCHLevelSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:

	UCHLevelSet();

	ATTRIBUTE_ACCESSORS(UCHLevelSet, Level);
	ATTRIBUTE_ACCESSORS(UCHLevelSet, EXP);

protected:
	
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_EXP(const FGameplayAttributeData& OldValue);

private:
	
	// The current level of the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Category = "CH|Level", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;

	// The current experience points of the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EXP, Category = "CH|Level", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EXP;
};
