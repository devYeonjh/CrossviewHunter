#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "CrossviewHunterGameplayCueTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Engine/TimerHandle.h"
#include "GameplayCueNotify_WeaponAttachedParticle.generated.h"

/**
 * GameplayCueNotify that activates/deactivates particle systems already attached to weapon actors
 * This approach doesn't spawn new actors but controls existing particle components on the weapon
 */
UCLASS()
class CROSSVIEWHUNTER_API AGameplayCueNotify_WeaponAttachedParticle : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGameplayCueNotify_WeaponAttachedParticle();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	/** Component name to search for on the weapon actor (optional - if empty, finds first available) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Particles")
	FName TargetComponentName;

	/** Whether to automatically deactivate after a certain time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Particles")
	bool bAutoDeactivate;

	/** Time before automatically deactivating the particle (only used if bAutoDeactivate is true) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Particles", meta = (EditCondition = "bAutoDeactivate", ClampMin = "0.0"))
	float AutoDeactivateDelay;

	/** Whether to reset particle system when activating (restart from beginning) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Particles")
	bool bResetOnActivate;

private:
	/** Timer handle for auto-deactivation */
	FTimerHandle AutoDeactivateTimerHandle;

	/** Find particle component on the target actor */
	USceneComponent* FindParticleComponent(AActor* TargetActor, FName ComponentName = NAME_None);

	/** Activate particle component */
	void ActivateParticleComponent(USceneComponent* ParticleComponent);

	/** Deactivate particle component */
	void DeactivateParticleComponent(USceneComponent* ParticleComponent);

	/** Timer callback for auto-deactivation */
	UFUNCTION()
	void AutoDeactivateParticle();

	/** Currently controlled particle component (for auto-deactivation) */
	UPROPERTY()
	TWeakObjectPtr<USceneComponent> ControlledParticleComponent;
};