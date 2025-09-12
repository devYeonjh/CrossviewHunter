// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LyraGame/AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CH_GA_ActivateFlameThrower.generated.h"

/**
 * UCrossviewHunterGameplayAbility_ActivateFlameThrower
 * 
 * GameplayAbility that activates particle systems on actors with "FlameThrower" tag attached to the player
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate FlameThrower"))
class CROSSVIEWHUNTER_API UCH_GA_ActivateFlameThrower : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:
	UCH_GA_ActivateFlameThrower(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

private:
	// Find actors with FlameThrower tag attached to the player
	UFUNCTION()
	TArray<AActor*> FindFlameThrowerActors(APawn* OwnerPawn) const;

	// Activate particle systems on the found actors
	UFUNCTION()
	void ActivateParticleSystems(const TArray<AActor*>& FlameThrowerActors);

	// Force restart particle component (alternative method)
	UFUNCTION()
	void ForceRestartParticleComponent(UParticleSystemComponent* ParticleComponent);

	// Store activated particle components for cleanup
	UPROPERTY()
	TArray<TObjectPtr<UParticleSystemComponent>> ActivatedParticleComponents;
};