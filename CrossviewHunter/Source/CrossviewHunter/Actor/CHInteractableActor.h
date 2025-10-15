// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularActor.h"
#include "Interaction/IInteractableTarget.h"
#include "CHInteractableActor.generated.h"

UCLASS()
class CROSSVIEWHUNTER_API ACHInteractableActor : public AModularActor, public IInteractableTarget
{
	GENERATED_BODY()

public:
	ACHInteractableActor();

	// ~IInteractableTarget Interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) override;
	// ~End of IInteractableTarget Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractionEvent(const FGameplayTag& InteractionEventTag, const FGameplayEventData& InOutEventData);

	void InteractionEvent_Implementation(const FGameplayTag& InteractionEventTag, const FGameplayEventData& InOutEventData);
	
protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

	

};
