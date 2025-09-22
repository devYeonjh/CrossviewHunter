// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#include "CHActorWithAbilities.h"

#include "CHActorAbilitySystemComponent.h"
#include "CHLogChannel.h" 


ACHActorWithAbilities::ACHActorWithAbilities(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Must replicate Actor for multiplayer
	bReplicates = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCHActorAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

void ACHActorWithAbilities::PostInitializeComponents()
{
	// BEFORE PostInit Components:
	InitializeAbilitySystem();

	// Now during PostInit Components, there is a functional ASC for other components to use
	Super::PostInitializeComponents();
}

void ACHActorWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilitySetOnSpawn)
	{
		AbilitySetOnSpawn->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandlesOnSpawn);
	}
}

void ACHActorWithAbilities::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();

	Super::EndPlay(EndPlayReason);
}

void ACHActorWithAbilities::InitializeAbilitySystem()
{
	UE_LOG(LogCH, Log, TEXT("Initializing Ability System"));

	// We expect this to have been set in the constructor
	check(IsValid(AbilitySystemComponent));

	// Initialize ASC on this Actor
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACHActorWithAbilities::UninitializeAbilitySystem()
{
	UE_LOG(LogCH, Log, TEXT("Uninitializing Ability System"));

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();
		AbilitySystemComponent->ClearActorInfo();

		GrantedHandlesOnSpawn.TakeFromAbilitySystem(AbilitySystemComponent);
	}
}

UAbilitySystemComponent* ACHActorWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ULyraAbilitySystemComponent* ACHActorWithAbilities::GetLyraAbilitySystemComponentChecked() const
{
	check(AbilitySystemComponent);
	return AbilitySystemComponent;
}
