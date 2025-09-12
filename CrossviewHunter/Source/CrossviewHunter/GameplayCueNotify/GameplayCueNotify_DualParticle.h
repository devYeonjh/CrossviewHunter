#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_ParticleActor.h"
#include "GameplayCueNotify_DualParticle.generated.h"

/**
 * Example GameplayCue that demonstrates using both Niagara and Legacy Particle Systems
 * This class shows how to set up different particle effects for different scenarios
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Dual Particle GameplayCue"))
class CROSSVIEWHUNTER_API AGameplayCueNotify_DualParticle : public AGameplayCueNotify_ParticleActor
{
	GENERATED_BODY()

public:
	AGameplayCueNotify_DualParticle();

protected:
	/** Additional particle info for different scenarios (e.g., impact vs explosion) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Particle System")
	FCrossviewHunter_ParticleInfo SecondaryParticleInfo;

	/** Whether to spawn secondary particle effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Particle System")
	bool bUseSecondaryEffect;

	/** Delay before spawning secondary effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Particle System", meta = (EditCondition = "bUseSecondaryEffect", ClampMin = "0.0"))
	float SecondaryEffectDelay;

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	/** Timer handle for delayed secondary effect */
	mutable FTimerHandle SecondaryEffectTimerHandle;

	/** Spawn secondary effect with delay */
	UFUNCTION()
	void SpawnSecondaryEffect(AActor* TargetActor, FGameplayCueParameters Parameters);
};