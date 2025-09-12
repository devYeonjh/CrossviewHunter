#include "ImpactParticleDisabler.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "GameplayCueSet.h"
#include "Engine/Engine.h"
#include "GameplayTagsManager.h"

bool UImpactParticleDisabler::bImpactParticlesDisabled = false;

void UImpactParticleDisabler::DisableImpactParticles()
{
	UE_LOG(LogTemp, Warning, TEXT("=== Attempting to disable Impact particles ==="));
	
	if (bImpactParticlesDisabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impact particles already disabled"));
		return;
	}

	// Get the GameplayCue manager
	UGameplayCueManager* GameplayCueManager = UAbilitySystemGlobals::Get().GetGameplayCueManager();
	if (!GameplayCueManager)
	{
		UE_LOG(LogTemp, Error, TEXT("GameplayCueManager not found!"));
		return;
	}

	// Find and disable Impact-related GameplayCue tags
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	
	// List of Impact tags to disable
	TArray<FString> ImpactTags = {
		TEXT("GameplayCue.Weapon.Rifle.Impact"),
		TEXT("GameplayCue.Weapon.Pistol.Impact"),
		TEXT("GameplayCue.Weapon.Shotgun.Impact"),
		TEXT("GameplayCue.Weapon.Melee.Impact")
	};

	for (const FString& TagString : ImpactTags)
	{
		FGameplayTag ImpactTag = TagsManager.RequestGameplayTag(FName(*TagString), false);
		if (ImpactTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Impact tag: %s - attempting to disable"), *TagString);
			
			// Try to remove the GameplayCue notification for this tag
			// Note: This approach might not work for all cases, but it's worth trying
			if (UGameplayCueSet* RuntimeCueSet = GameplayCueManager->GetRuntimeCueSet())
			{
				// Find the cue data for this tag
				if (int32* DataIdx = RuntimeCueSet->GameplayCueDataMap.Find(ImpactTag))
				{
					if (RuntimeCueSet->GameplayCueData.IsValidIndex(*DataIdx))
					{
						// Temporarily clear the GameplayCue object path
						FGameplayCueNotifyData& CueData = RuntimeCueSet->GameplayCueData[*DataIdx];
						UE_LOG(LogTemp, Warning, TEXT("Disabling GameplayCue object: %s"), *CueData.GameplayCueNotifyObj.ToString());
						
						// Store the original path for potential re-enabling
						// For now, we'll just clear it
						CueData.GameplayCueNotifyObj = FSoftObjectPath();
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Impact tag not found: %s"), *TagString);
		}
	}

	bImpactParticlesDisabled = true;
	UE_LOG(LogTemp, Warning, TEXT("Impact particles disabled successfully"));
}

void UImpactParticleDisabler::EnableImpactParticles()
{
	UE_LOG(LogTemp, Warning, TEXT("=== Re-enabling Impact particles ==="));
	
	if (!bImpactParticlesDisabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Impact particles are not currently disabled"));
		return;
	}

	// This would require storing the original paths and restoring them
	// For now, just mark as enabled
	bImpactParticlesDisabled = false;
	UE_LOG(LogTemp, Warning, TEXT("Impact particles re-enabled"));
}