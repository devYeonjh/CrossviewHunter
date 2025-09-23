// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#include "CHActorWithAbilitiesAndHealth.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraHealthComponent.h"


ACHActorWithAbilitiesAndHealth::ACHActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CombatSet = ObjectInitializer.CreateDefaultSubobject<ULyraCombatSet>(this, TEXT("CombatSet"));
	HealthSet = ObjectInitializer.CreateDefaultSubobject<ULyraHealthSet>(this, TEXT("HealthSet"));

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<ULyraHealthComponent>(this, TEXT("HealthComponent"));
}

void ACHActorWithAbilitiesAndHealth::InitializeAbilitySystem()
{
	// ASC가 작동하려면 Super::InitializeAbilitySystem이 필요하므로 먼저 실행:
	Super::InitializeAbilitySystem();

	// HealthComponent는 Lyra 기반 ASC가 필요
	ULyraAbilitySystemComponent* ASC = GetLyraAbilitySystemComponentChecked();

	// ASC에 Attribute Set 추가
	ASC->AddAttributeSetSubobject(CombatSet.Get());
	ASC->AddAttributeSetSubobject(HealthSet.Get());

	// !! HealthSet이 추가된 후에만 HealthComponent를 초기화하세요
	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void ACHActorWithAbilitiesAndHealth::UninitializeAbilitySystem()
{
	HealthComponent->UninitializeFromAbilitySystem();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveSpawnedAttribute(CombatSet.Get());
		ASC->RemoveSpawnedAttribute(HealthSet.Get());
	}

	Super::UninitializeAbilitySystem();
}
