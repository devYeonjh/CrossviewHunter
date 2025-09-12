#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "CrossviewHunterGameplayCueTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Engine/TimerHandle.h"
#include "GameplayCueNotify_ParticleActor.generated.h"

UCLASS()
class CROSSVIEWHUNTER_API AGameplayCueNotify_ParticleActor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGameplayCueNotify_ParticleActor();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	/** Particle system information (supports both Niagara and Legacy systems) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	FCrossviewHunter_ParticleInfo ParticleInfo;

	/** Legacy particle system component (for backwards compatibility) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Legacy", meta = (EditCondition = "false"))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

	/** Niagara system component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (EditCondition = "false"))
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	/** Legacy particle system template (for backwards compatibility) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legacy", meta = (EditCondition = "false"))
	TObjectPtr<class UParticleSystem> ParticleSystemTemplate;

	/** Legacy destroy delay (for backwards compatibility) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legacy", meta = (EditCondition = "false"))
	float DestroyDelay;

	/** Information about the currently spawned particle system */
	UPROPERTY()
	FCrossviewHunter_SpawnedParticleInfo SpawnedParticleInfo;

public:
	/** Spawn a particle system based on the ParticleInfo configuration */
	UFUNCTION(BlueprintCallable, Category = "Particle System")
	FCrossviewHunter_SpawnedParticleInfo SpawnParticleSystem(AActor* TargetActor, const FGameplayCueParameters& Parameters);

	/** Spawn a particle system using the provided ParticleInfo */
	UFUNCTION(BlueprintCallable, Category = "Particle System")
	static FCrossviewHunter_SpawnedParticleInfo SpawnParticleSystemStatic(const FCrossviewHunter_ParticleInfo& InParticleInfo, AActor* TargetActor, const FGameplayCueParameters& Parameters);

private:
	FTimerHandle DestroyTimerHandle;

	UFUNCTION()
	void DestroyParticleActor();

	/** Initialize from legacy settings (for backwards compatibility) */
	void InitializeLegacySettings();

	/** Get the effective transform for spawning particles */
	FTransform GetParticleSpawnTransform(AActor* TargetActor, const FGameplayCueParameters& Parameters, const FCrossviewHunter_ParticleInfo& InParticleInfo);
};