// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LyraGame/AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Components/ActorComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CH_GA_DeactivateFlameThrower.generated.h"

/**
 * UCrossviewHunterGameplayAbility_DeactivateFlameThrower
 * 
 * GameplayAbility that deactivates particle systems on actors with "FlameThrower" tag attached to the player
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deactivate FlameThrower"))
class CROSSVIEWHUNTER_API UCH_GA_DeactivateFlameThrower : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:
	UCH_GA_DeactivateFlameThrower(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

private:
	// Find actors with FlameThrower tag attached to the player
	UFUNCTION()
	TArray<AActor*> FindFlameThrowerActors(APawn* OwnerPawn) const;

	// Deactivate particle systems on the found actors
	UFUNCTION()
	void DeactivateParticleSystems(const TArray<AActor*>& FlameThrowerActors);
};