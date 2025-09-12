// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH_GA_DeactivateFlameThrower.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

UCH_GA_DeactivateFlameThrower::UCH_GA_DeactivateFlameThrower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set default activation policy
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = ELyraAbilityActivationGroup::Independent;
}

void UCH_GA_DeactivateFlameThrower::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!OwnerPawn)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Find all FlameThrower actors attached to the player
	TArray<AActor*> FlameThrowerActors = FindFlameThrowerActors(OwnerPawn);
	
	if (FlameThrowerActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FlameThrower actors found attached to player"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Deactivate particle systems on found actors
	DeactivateParticleSystems(FlameThrowerActors);

	UE_LOG(LogTemp, Log, TEXT("FlameThrower particles deactivated on %d actors"), FlameThrowerActors.Num());

	// Call parent implementation
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// End the ability immediately after deactivation
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCH_GA_DeactivateFlameThrower::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Call parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TArray<AActor*> UCH_GA_DeactivateFlameThrower::FindFlameThrowerActors(APawn* OwnerPawn) const
{
	TArray<AActor*> FlameThrowerActors;
	
	if (!OwnerPawn)
	{
		return FlameThrowerActors;
	}

	// Get all attached actors
	TArray<AActor*> AttachedActors;
	OwnerPawn->GetAttachedActors(AttachedActors);

	// Filter actors with "FlameThrower" tag
	for (AActor* AttachedActor : AttachedActors)
	{
		if (AttachedActor && AttachedActor->ActorHasTag(FName("FlameThrower")))
		{
			FlameThrowerActors.Add(AttachedActor);
		}
	}

	return FlameThrowerActors;
}

void UCH_GA_DeactivateFlameThrower::DeactivateParticleSystems(const TArray<AActor*>& FlameThrowerActors)
{
	for (AActor* FlameThrowerActor : FlameThrowerActors)
	{
		if (!FlameThrowerActor)
		{
			continue;
		}

		// Find all particle system components in the actor
		TArray<UParticleSystemComponent*> ParticleComponents;
		FlameThrowerActor->GetComponents<UParticleSystemComponent>(ParticleComponents);

		for (UParticleSystemComponent* ParticleComponent : ParticleComponents)
		{
			if (ParticleComponent && ParticleComponent->IsActive())
			{
				// Gentle deactivation that allows for easy reactivation
				ParticleComponent->Deactivate();
				
				UE_LOG(LogTemp, Log, TEXT("Gently deactivated particle system on actor: %s, IsActive: %s"), 
					*FlameThrowerActor->GetName(), 
					ParticleComponent->IsActive() ? TEXT("True") : TEXT("False"));
			}
		}
	}
}