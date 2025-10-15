// Fill out your copyright notice in the Description page of Project Settings.


#include "CHInteractableActor.h"


// Sets default values
ACHInteractableActor::ACHInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACHInteractableActor::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

void ACHInteractableActor::CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag,
	FGameplayEventData& InOutEventData)
{
	InteractionEvent_Implementation(InteractionEventTag,InOutEventData);
}

void ACHInteractableActor::InteractionEvent_Implementation(const FGameplayTag& InteractionEventTag, const FGameplayEventData& InOutEventData)
{
	InteractionEvent(InteractionEventTag, InOutEventData);
}
