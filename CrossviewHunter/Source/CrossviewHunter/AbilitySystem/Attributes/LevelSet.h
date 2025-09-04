// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"

#include "LevelSet.generated.h"

class UObject;
struct FFrame;

/**
 * ULevelSet
 *
 *	레벨 및 경험치 관련 스탯을 정의하는 클래스입니다.
 *	Attribute 예시: 레벨, 경험치
 */
UCLASS(BlueprintType)
class CROSSVIEWHUNTER_API ULevelSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:

	ULevelSet();

	ATTRIBUTE_ACCESSORS(ULevelSet, Level);
	ATTRIBUTE_ACCESSORS(ULevelSet, EXP);

protected:
	
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_EXP(const FGameplayAttributeData& OldValue);

private:
	
	// The current level of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Category = "CH|Level", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;

	// The current experience points of the character.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EXP, Category = "CH|Level", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EXP;
};
