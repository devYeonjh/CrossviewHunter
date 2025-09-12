#pragma once

#include "CoreMinimal.h"
#include "GameplayCueInterface.h"
#include "CrossviewHunterGameplayCueTypes.generated.h"

class UNiagaraSystem;
class UParticleSystem;
class UNiagaraComponent;
class UParticleSystemComponent;

UENUM(BlueprintType)
enum class EParticleSystemType : uint8
{
	None		UMETA(DisplayName = "None"),
	Niagara		UMETA(DisplayName = "Niagara System"),
	Legacy		UMETA(DisplayName = "Legacy Particle System"),
	Both		UMETA(DisplayName = "Both (Priority: Niagara)")
};

/**
 * Enhanced struct containing information for spawning particle effects in GameplayCue notifications
 * Supports both Niagara and Legacy Particle Systems (extends engine functionality)
 */
USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FCrossviewHunter_ParticleInfo
{
	GENERATED_BODY()

public:
	FCrossviewHunter_ParticleInfo()
		: ParticleSystemType(EParticleSystemType::Niagara)
		, NiagaraSystem(nullptr)
		, ParticleSystem(nullptr)
		, bAutoDestroy(true)
		, DestroyDelay(1.0f)
		, bAttachToTarget(false)
		, AttachPointName(NAME_None)
		, LocationOffset(FVector::ZeroVector)
		, RotationOffset(FRotator::ZeroRotator)
		, Scale(FVector::OneVector)
	{
	}

	/** Type of particle system to use */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
	EParticleSystemType ParticleSystemType;

	/** Niagara system to spawn (modern particle system) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System", meta = (EditCondition = "ParticleSystemType == EParticleSystemType::Niagara || ParticleSystemType == EParticleSystemType::Both"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	/** Legacy particle system to spawn (fallback or preference) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System", meta = (EditCondition = "ParticleSystemType == EParticleSystemType::Legacy || ParticleSystemType == EParticleSystemType::Both"))
	TObjectPtr<UParticleSystem> ParticleSystem;

	/** Whether the particle system should be automatically destroyed after playing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle")
	uint8 bAutoDestroy : 1;

	/** Delay before destroying the particle system (only used if bAutoDestroy is true) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (EditCondition = "bAutoDestroy", ClampMin = "0.0"))
	float DestroyDelay;

	/** Whether to attach the particle system to the target actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	uint8 bAttachToTarget : 1;

	/** Socket or bone name to attach to (only used if bAttachToTarget is true) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform", meta = (EditCondition = "bAttachToTarget"))
	FName AttachPointName;

	/** Location offset from target or world position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector LocationOffset;

	/** Rotation offset from target or world rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FRotator RotationOffset;

	/** Scale to apply to the particle system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector Scale;

	/**
	 * Get the effective particle system to use based on availability and settings
	 * @param OutSystemType The actual system type that will be used
	 * @return True if a valid system is available
	 */
	bool GetEffectiveParticleSystem(EParticleSystemType& OutSystemType) const;

	/**
	 * Check if this particle info is valid (has at least one system configured)
	 */
	bool IsValid() const;

	/**
	 * Get display name for the configured particle systems
	 */
	FString GetDisplayName() const;
};

/**
 * Helper struct for managing spawned particle components
 */
USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FCrossviewHunter_SpawnedParticleInfo
{
	GENERATED_BODY()

public:
	FCrossviewHunter_SpawnedParticleInfo()
		: NiagaraComponent(nullptr)
		, ParticleComponent(nullptr)
		, SpawnedSystemType(EParticleSystemType::None)
	{
	}

	/** Spawned Niagara component (if using Niagara system) */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	/** Spawned particle system component (if using legacy system) */
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	/** Type of system that was actually spawned */
	UPROPERTY()
	EParticleSystemType SpawnedSystemType;

	/**
	 * Check if this spawned info contains a valid component
	 */
	bool IsValid() const;

	/**
	 * Get the root component (works for both system types)
	 */
	USceneComponent* GetRootComponent() const;

	/**
	 * Activate the spawned particle system
	 */
	void Activate();

	/**
	 * Deactivate the spawned particle system
	 */
	void Deactivate();

	/**
	 * Destroy the spawned particle system
	 */
	void DestroyComponent();
};