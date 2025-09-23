// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "ModularActor.h"
#include "CHActorWithAbilities.generated.h"

class UCHActorAbilitySystemComponent;

UCLASS(Blueprintable)
class CROSSVIEWHUNTER_API ACHActorWithAbilities : public AModularActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** 스폰 시 이 액터에게 부여할 AbilitySet (있는 경우) */
	UPROPERTY(EditDefaultsOnly, Category="CH|Ability")
	TObjectPtr<ULyraAbilitySet> AbilitySetOnSpawn;
	
public:
	// Set Class Defaults
	ACHActorWithAbilities(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~AActor implementation
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor implementation

	/**
	 * 액터의 ASC를 초기화할 때 호출되는 함수
	 *
	 * 필요에 따라 오버라이드하세요. 오버라이드할 때는 ASC를 사용하기 **전에**
	 * 반드시 Super()를 호출해야 합니다. Super()를 호출하기 전까지는 ASC가
	 * 정상적으로 작동하지 않습니다.
	 */
	virtual void InitializeAbilitySystem();

	/**
	 * 액터의 ASC를 해제할 때 호출되는 함수
	 */
	virtual void UninitializeAbilitySystem();

public:
	//~IAbilitySystemComponent interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponentChecked() const;
	//~End IAbilitySystemComponent interface


private:
	//  The ability system component for this actor
	UPROPERTY(EditDefaultsOnly, Category="CH|Ability")
	TObjectPtr<UCHActorAbilitySystemComponent> AbilitySystemComponent;

	// 스폰 시 이 액터에게 부여된 핸들 목록; EndPlay에서 정리
	UPROPERTY(VisibleInstanceOnly, Category="CH|Ability")
	FLyraAbilitySet_GrantedHandles GrantedHandlesOnSpawn;
};
