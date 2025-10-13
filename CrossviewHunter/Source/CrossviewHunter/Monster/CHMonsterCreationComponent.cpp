// Fill out your copyright notice in the Description page of Project Settings.


#include "CHMonsterCreationComponent.h"


UCHMonsterCreationComponent::UCHMonsterCreationComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCHMonsterCreationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCHMonsterCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
}
