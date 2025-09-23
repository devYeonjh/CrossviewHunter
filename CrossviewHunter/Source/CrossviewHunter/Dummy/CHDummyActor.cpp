// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#include "CHDummyActor.h"

#include "CHDummyHealthSet.h"
#include "CHLogChannel.h"

ACHDummyActor::ACHDummyActor(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UCHDummyHealthSet>("HealthSet")
		)
{
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "MeshComponent");
	RootComponent = MeshComponent;
}

void ACHDummyActor::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();

	// Health Set 기본값 초기화

	// Super::InitializeAbilitySystem() 이전에 실행하면 실패합니다
	// 이 때문에 생성자로 옮길 수 없습니다.

	HealthSet->InitMaxHealth(100.f);
	HealthSet->InitHealth(1.f);
}

void ACHDummyActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogCH, Log, TEXT("BeginPlay with Health = %.1f/%.1f"), HealthSet->GetHealth(), HealthSet->GetMaxHealth());
}
