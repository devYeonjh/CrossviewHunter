#include "GameplayCueNotify_DualParticle.h"
#include "Engine/World.h"
#include "TimerManager.h"

AGameplayCueNotify_DualParticle::AGameplayCueNotify_DualParticle()
{
	// Set up primary particle info (Niagara by default)
	ParticleInfo.ParticleSystemType = EParticleSystemType::Niagara;
	ParticleInfo.bAutoDestroy = true;
	ParticleInfo.DestroyDelay = 2.0f;
	ParticleInfo.bAttachToTarget = false;

	// Set up secondary particle info (Legacy system for variety)
	SecondaryParticleInfo.ParticleSystemType = EParticleSystemType::Legacy;
	SecondaryParticleInfo.bAutoDestroy = true;
	SecondaryParticleInfo.DestroyDelay = 1.5f;
	SecondaryParticleInfo.bAttachToTarget = true;
	SecondaryParticleInfo.LocationOffset = FVector(0.0f, 0.0f, 50.0f); // Spawn slightly above

	// Configuration
	bUseSecondaryEffect = false;
	SecondaryEffectDelay = 0.5f;
}

bool AGameplayCueNotify_DualParticle::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// First, execute the primary particle effect using parent implementation
	bool bPrimarySuccess = Super::OnExecute_Implementation(MyTarget, Parameters);

	// If enabled, spawn secondary effect with delay
	if (bUseSecondaryEffect && SecondaryParticleInfo.IsValid() && MyTarget)
	{
		UWorld* World = MyTarget->GetWorld();
		if (World)
		{
			// Use a timer to delay the secondary effect
			FTimerDelegate SecondaryEffectDelegate;
			SecondaryEffectDelegate.BindUFunction(
				this,
				FName("SpawnSecondaryEffect"),
				MyTarget,
				Parameters
			);

			World->GetTimerManager().SetTimer(
				SecondaryEffectTimerHandle,
				SecondaryEffectDelegate,
				SecondaryEffectDelay,
				false
			);
		}
	}

	return bPrimarySuccess;
}

void AGameplayCueNotify_DualParticle::SpawnSecondaryEffect(AActor* TargetActor, FGameplayCueParameters Parameters)
{
	if (TargetActor && SecondaryParticleInfo.IsValid())
	{
		// Spawn the secondary particle system using our static helper function
		FCrossviewHunter_SpawnedParticleInfo SpawnedSecondary = SpawnParticleSystemStatic(SecondaryParticleInfo, TargetActor, Parameters);
		
		if (SpawnedSecondary.IsValid())
		{
			SpawnedSecondary.Activate();
			
			// The secondary effect will auto-destroy based on its own settings
		}
	}
}