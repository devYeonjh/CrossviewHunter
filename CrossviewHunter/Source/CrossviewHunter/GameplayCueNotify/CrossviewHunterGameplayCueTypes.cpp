#include "CrossviewHunterGameplayCueTypes.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

bool FCrossviewHunter_ParticleInfo::GetEffectiveParticleSystem(EParticleSystemType& OutSystemType) const
{
	switch (ParticleSystemType)
	{
		case EParticleSystemType::Niagara:
			if (NiagaraSystem)
			{
				OutSystemType = EParticleSystemType::Niagara;
				return true;
			}
			break;

		case EParticleSystemType::Legacy:
			if (ParticleSystem)
			{
				OutSystemType = EParticleSystemType::Legacy;
				return true;
			}
			break;

		case EParticleSystemType::Both:
			// Priority: Niagara first, then Legacy
			if (NiagaraSystem)
			{
				OutSystemType = EParticleSystemType::Niagara;
				return true;
			}
			else if (ParticleSystem)
			{
				OutSystemType = EParticleSystemType::Legacy;
				return true;
			}
			break;

		case EParticleSystemType::None:
		default:
			break;
	}

	OutSystemType = EParticleSystemType::None;
	return false;
}

bool FCrossviewHunter_ParticleInfo::IsValid() const
{
	switch (ParticleSystemType)
	{
		case EParticleSystemType::Niagara:
			return NiagaraSystem != nullptr;

		case EParticleSystemType::Legacy:
			return ParticleSystem != nullptr;

		case EParticleSystemType::Both:
			return (NiagaraSystem != nullptr) || (ParticleSystem != nullptr);

		case EParticleSystemType::None:
		default:
			return false;
	}
}

FString FCrossviewHunter_ParticleInfo::GetDisplayName() const
{
	TArray<FString> SystemNames;
	
	if (NiagaraSystem)
	{
		SystemNames.Add(FString::Printf(TEXT("Niagara: %s"), *NiagaraSystem->GetName()));
	}
	
	if (ParticleSystem)
	{
		SystemNames.Add(FString::Printf(TEXT("Legacy: %s"), *ParticleSystem->GetName()));
	}

	if (SystemNames.Num() == 0)
	{
		return TEXT("No Particle Systems");
	}

	return FString::Join(SystemNames, TEXT(", "));
}

bool FCrossviewHunter_SpawnedParticleInfo::IsValid() const
{
	switch (SpawnedSystemType)
	{
		case EParticleSystemType::Niagara:
			return NiagaraComponent != nullptr && ::IsValid(NiagaraComponent);

		case EParticleSystemType::Legacy:
			return ParticleComponent != nullptr && ::IsValid(ParticleComponent);

		default:
			return false;
	}
}

USceneComponent* FCrossviewHunter_SpawnedParticleInfo::GetRootComponent() const
{
	switch (SpawnedSystemType)
	{
		case EParticleSystemType::Niagara:
			return NiagaraComponent;

		case EParticleSystemType::Legacy:
			return ParticleComponent;

		default:
			return nullptr;
	}
}

void FCrossviewHunter_SpawnedParticleInfo::Activate()
{
	switch (SpawnedSystemType)
	{
		case EParticleSystemType::Niagara:
			if (NiagaraComponent)
			{
				NiagaraComponent->Activate(true);
			}
			break;

		case EParticleSystemType::Legacy:
			if (ParticleComponent)
			{
				ParticleComponent->Activate(true);
			}
			break;

		default:
			break;
	}
}

void FCrossviewHunter_SpawnedParticleInfo::Deactivate()
{
	switch (SpawnedSystemType)
	{
		case EParticleSystemType::Niagara:
			if (NiagaraComponent)
			{
				NiagaraComponent->Deactivate();
			}
			break;

		case EParticleSystemType::Legacy:
			if (ParticleComponent)
			{
				ParticleComponent->Deactivate();
			}
			break;

		default:
			break;
	}
}

void FCrossviewHunter_SpawnedParticleInfo::DestroyComponent()
{
	switch (SpawnedSystemType)
	{
		case EParticleSystemType::Niagara:
			if (NiagaraComponent)
			{
				NiagaraComponent->DestroyComponent();
				NiagaraComponent = nullptr;
			}
			break;

		case EParticleSystemType::Legacy:
			if (ParticleComponent)
			{
				ParticleComponent->DestroyComponent();
				ParticleComponent = nullptr;
			}
			break;

		default:
			break;
	}

	SpawnedSystemType = EParticleSystemType::None;
}