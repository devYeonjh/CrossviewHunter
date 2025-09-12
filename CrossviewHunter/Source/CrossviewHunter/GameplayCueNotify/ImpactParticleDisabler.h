#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "ImpactParticleDisabler.generated.h"

/**
 * Helper class to disable Impact particle GameplayCues
 */
UCLASS()
class CROSSVIEWHUNTER_API UImpactParticleDisabler : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Initialize the Impact particle disabler
	 * This should be called during game initialization
	 */
	UFUNCTION(BlueprintCallable, Category = "Impact Particles")
	static void DisableImpactParticles();

	/**
	 * Re-enable Impact particles if needed
	 */
	UFUNCTION(BlueprintCallable, Category = "Impact Particles")
	static void EnableImpactParticles();

private:
	static bool bImpactParticlesDisabled;
};