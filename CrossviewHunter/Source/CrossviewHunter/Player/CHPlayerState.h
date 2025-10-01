// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LyraPlayerState.h"
#include "CHPlayerState.generated.h"

/**
 * Lyra Player State 확장
 * Experience 로드시 플레이어 정보 저장기능 추가
 */
UCLASS()
class CROSSVIEWHUNTER_API ACHPlayerState : public ALyraPlayerState
{
	GENERATED_BODY()

public:
	ACHPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~APlayerState override
	virtual void OnDeactivated() override;
	//~End of APlayerState override
	
	//~ALyraPlayerState override 
	virtual void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience) override;
	virtual void InitializeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) override;
	//~End of ALyraPlayerState override

	// 사용하고 있는 StatSet
	UPROPERTY()
	TObjectPtr<const class UCHStatSet> StatSet;
	// 사용하고 있는 GoldSet
	UPROPERTY()
	TObjectPtr<const class UCHGoldSet> GoldSet;
	// 사용하고 있는 LevelSet
	UPROPERTY()
	TObjectPtr<const class UCHLevelSet> LevelSet;
};
