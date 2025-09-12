#include "GameplayCueNotify_WeaponAttachedParticle.h"
#include "CrossviewHunterGameplayCueTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

AGameplayCueNotify_WeaponAttachedParticle::AGameplayCueNotify_WeaponAttachedParticle()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize default settings
	TargetComponentName = NAME_None; // Will search for first available particle component
	bAutoDeactivate = true;
	AutoDeactivateDelay = 2.0f; // Default 2 seconds
	bResetOnActivate = true;

	// Allow multiple instances per instigator
	bAutoDestroyOnRemove = false;
	bUniqueInstancePerInstigator = false;
	bUniqueInstancePerSourceObject = false;
}

bool AGameplayCueNotify_WeaponAttachedParticle::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("=== WeaponAttachedParticle OnExecute CALLED! ==="));

	if (!MyTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponAttachedParticle: MyTarget is null!"));
		return false;
	}

	// Find particle component on the weapon actor
	USceneComponent* ParticleComponent = FindParticleComponent(MyTarget, TargetComponentName);
	if (!ParticleComponent)
	{
	UE_LOG(LogTemp, Error, TEXT("WeaponAttachedParticle: No particle component found on %s"), *MyTarget->GetName());
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Found particle component: %s"), *ParticleComponent->GetName());

	// Activate the particle component
	ActivateParticleComponent(ParticleComponent);

	// Store reference for auto-deactivation
	ControlledParticleComponent = ParticleComponent;

	// Set up auto-deactivation timer if enabled
	if (bAutoDeactivate && AutoDeactivateDelay > 0.0f)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Setting auto-deactivate timer for %f seconds"), AutoDeactivateDelay);
			World->GetTimerManager().SetTimer(
				AutoDeactivateTimerHandle,
				this,
				&AGameplayCueNotify_WeaponAttachedParticle::AutoDeactivateParticle,
				AutoDeactivateDelay,
				false
			);
		}
	}

	return true;
}

bool AGameplayCueNotify_WeaponAttachedParticle::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("=== WeaponAttachedParticle OnRemove CALLED! ==="));

	if (!MyTarget)
	{
		return false;
	}

	// Find particle component and deactivate it
	USceneComponent* ParticleComponent = FindParticleComponent(MyTarget, TargetComponentName);
	if (ParticleComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Deactivating particle component: %s"), *ParticleComponent->GetName());
		DeactivateParticleComponent(ParticleComponent);
	}

	// Clear auto-deactivation timer
	if (AutoDeactivateTimerHandle.IsValid())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(AutoDeactivateTimerHandle);
		}
	}

	return true;
}

USceneComponent* AGameplayCueNotify_WeaponAttachedParticle::FindParticleComponent(AActor* TargetActor, FName ComponentName)
{
	if (!TargetActor)
	{
		return nullptr;
	}

	USceneComponent* FoundComponent = nullptr;

	// If specific component name is provided, search for it
	if (ComponentName != NAME_None)
	{
		FoundComponent = TargetActor->FindComponentByClass<USceneComponent>();
		TArray<USceneComponent*> SceneComponents;
		TargetActor->GetComponents<USceneComponent>(SceneComponents);
		
		for (USceneComponent* Component : SceneComponents)
		{
			if (Component && Component->GetFName() == ComponentName)
			{
				// Check if it's a particle component
				if (Component->IsA<UNiagaraComponent>() || Component->IsA<UParticleSystemComponent>())
				{
					FoundComponent = Component;
					break;
				}
			}
		}
	}
	else
	{
		// Search for first available Niagara component
		FoundComponent = TargetActor->FindComponentByClass<UNiagaraComponent>();
		
		// If no Niagara component found, search for Legacy particle component
		if (!FoundComponent)
		{
			FoundComponent = TargetActor->FindComponentByClass<UParticleSystemComponent>();
		}
	}

	return FoundComponent;
}

void AGameplayCueNotify_WeaponAttachedParticle::ActivateParticleComponent(USceneComponent* ParticleComponent)
{
	if (!ParticleComponent)
	{
		return;
	}

	// Handle Niagara Component
	if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(ParticleComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Activating Niagara component"));
		
		if (bResetOnActivate)
		{
			NiagaraComp->ResetSystem();
		}
		
		NiagaraComp->SetVisibility(true);
		NiagaraComp->SetActive(true);
		NiagaraComp->Activate();
	}
	// Handle Legacy Particle Component
	else if (UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(ParticleComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Activating Legacy particle component"));
		
		if (bResetOnActivate)
		{
			ParticleComp->ResetToDefaults();
		}
		
		ParticleComp->SetVisibility(true);
		ParticleComp->SetActive(true);
		ParticleComp->Activate();
	}
}

void AGameplayCueNotify_WeaponAttachedParticle::DeactivateParticleComponent(USceneComponent* ParticleComponent)
{
	if (!ParticleComponent)
	{
		return;
	}

	// Handle Niagara Component
	if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(ParticleComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Deactivating Niagara component"));
		NiagaraComp->SetActive(false);
		NiagaraComp->Deactivate();
	}
	// Handle Legacy Particle Component
	else if (UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(ParticleComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Deactivating Legacy particle component"));
		ParticleComp->SetActive(false);
		ParticleComp->Deactivate();
	}
}

void AGameplayCueNotify_WeaponAttachedParticle::AutoDeactivateParticle()
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponAttachedParticle: Auto-deactivating particle"));

	if (ControlledParticleComponent.IsValid())
	{
		DeactivateParticleComponent(ControlledParticleComponent.Get());
		ControlledParticleComponent.Reset();
	}

	// Clear the timer handle
	AutoDeactivateTimerHandle.Invalidate();
}