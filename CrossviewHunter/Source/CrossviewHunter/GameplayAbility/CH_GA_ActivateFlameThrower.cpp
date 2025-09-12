// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH_GA_ActivateFlameThrower.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

UCH_GA_ActivateFlameThrower::UCH_GA_ActivateFlameThrower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set default activation policy
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = ELyraAbilityActivationGroup::Independent;
}

bool UCH_GA_ActivateFlameThrower::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	UE_LOG(LogTemp, Warning, TEXT("=== CanActivateAbility called: %s ==="), bCanActivate ? TEXT("TRUE") : TEXT("FALSE"));
	return bCanActivate;
}

void UCH_GA_ActivateFlameThrower::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("=== UCH_GA_ActivateFlameThrower::ActivateAbility CALLED ==="));
	
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ActorInfo or AvatarActor"));
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

	// Activate particle systems on found actors
	ActivateParticleSystems(FlameThrowerActors);

	UE_LOG(LogTemp, Log, TEXT("FlameThrower particles activated on %d actors"), FlameThrowerActors.Num());

	// Call parent implementation
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// End the ability immediately after activation (instant ability)
	UE_LOG(LogTemp, Warning, TEXT("=== Ending ActivateFlameThrower Ability ==="));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCH_GA_ActivateFlameThrower::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Clear the reference to activated particle components
	ActivatedParticleComponents.Empty();

	// Call parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TArray<AActor*> UCH_GA_ActivateFlameThrower::FindFlameThrowerActors(APawn* OwnerPawn) const
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

void UCH_GA_ActivateFlameThrower::ActivateParticleSystems(const TArray<AActor*>& FlameThrowerActors)
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
			if (ParticleComponent)
			{
				ParticleComponent->SetActive(true);
				
				// Store reference for cleanup
				ActivatedParticleComponents.Add(ParticleComponent);
				
				UE_LOG(LogTemp, Log, TEXT("Force activated particle system on actor: %s, IsActive: %s"), 
					*FlameThrowerActor->GetName(), 
					ParticleComponent->IsActive() ? TEXT("True") : TEXT("False"));
			}
		}
	}
}

void UCH_GA_ActivateFlameThrower::ForceRestartParticleComponent(UParticleSystemComponent* ParticleComponent)
{
	if (!ParticleComponent)
	{
		return;
	}

	// Alternative method: Complete component recreation approach
	// Step 1: Store current settings
	bool bWasAutoActivate = ParticleComponent->bAutoActivate;
	UParticleSystem* ParticleTemplate = ParticleComponent->Template;
	
	// Step 2: Complete shutdown
	ParticleComponent->DeactivateSystem();
	ParticleComponent->SetActive(false);
	
	// Step 3: Wait one frame (using timer)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, ParticleComponent, bWasAutoActivate, ParticleTemplate]()
	{
		if (ParticleComponent && IsValid(ParticleComponent))
		{
			// Step 4: Restore and restart
			ParticleComponent->bAutoActivate = true;
			ParticleComponent->SetActive(true);
			ParticleComponent->ActivateSystem(true);
			
			UE_LOG(LogTemp, Log, TEXT("Force restarted particle component"));
		}
	}, 0.1f, false);
}