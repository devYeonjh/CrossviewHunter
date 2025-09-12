#include "GameplayCueNotify_ParticleActor.h"
#include "CrossviewHunterGameplayCueTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagContainer.h"

AGameplayCueNotify_ParticleActor::AGameplayCueNotify_ParticleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a simple scene component as root first
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	// Create both component types but don't set templates yet
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->bAutoActivate = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false;

	// Legacy settings for backwards compatibility
	DestroyDelay = 1.0f;
	
	// 여러 인스턴스가 동시에 존재할 수 있도록 설정
	bAutoDestroyOnRemove = false;
	bUniqueInstancePerInstigator = false;
	bUniqueInstancePerSourceObject = false;

	// Initialize default particle info
	ParticleInfo.ParticleSystemType = EParticleSystemType::Niagara;
	ParticleInfo.bAutoDestroy = true;
	ParticleInfo.DestroyDelay = 1.0f;
	ParticleInfo.bAttachToTarget = false;
	ParticleInfo.LocationOffset = FVector::ZeroVector;
	ParticleInfo.RotationOffset = FRotator::ZeroRotator;
	ParticleInfo.Scale = FVector::OneVector;
}


bool AGameplayCueNotify_ParticleActor::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	// 디버깅용 로그 - OnExecute가 호출되는지 확인
	UE_LOG(LogTemp, Error, TEXT("=== OnExecute_Implementation CALLED! ==="));
	
	// Check if this is an Impact GameplayCue and skip particle spawning if so
	FString GameplayCueTagString = GameplayCueTag.ToString();
	UE_LOG(LogTemp, Warning, TEXT("GameplayCue Tag: %s"), *GameplayCueTagString);
	
	if (GameplayCueTagString.Contains(TEXT("Impact")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Impact GameplayCue detected - skipping particle spawn"));
		return false;
	}

	// Force 1 second auto-destroy settings for new actors
	ParticleInfo.bAutoDestroy = true;
	ParticleInfo.DestroyDelay = 1.0f;

	// Check if we have a valid particle system configuration
	if (!ParticleInfo.IsValid() && !ParticleSystemTemplate)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid particle system configured! Aborting."));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Valid particle system found, proceeding with spawn..."));

	if (MyTarget)
	{
		UWorld* World = MyTarget->GetWorld();
		if (World)
		{
			// Use simple transform for actor spawning - actual particle transform will be calculated in SpawnParticleSystemStatic
			FVector ActorLocation = Parameters.Location.IsNearlyZero() ? MyTarget->GetActorLocation() : FVector(Parameters.Location);
			FTransform SpawnTransform(MyTarget->GetActorRotation(), ActorLocation);
			
			// 새로운 파티클 액터 인스턴스 생성
			AGameplayCueNotify_ParticleActor* NewParticleActor = World->SpawnActor<AGameplayCueNotify_ParticleActor>(
				GetClass(),
				SpawnTransform
			);
			
			if (NewParticleActor && NewParticleActor != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("New particle actor spawned successfully: %s"), *NewParticleActor->GetName());
				
				// Copy particle configuration to new instance
				NewParticleActor->ParticleInfo = ParticleInfo;
				
				// Legacy compatibility: if using old system, copy the template
				if (ParticleSystemTemplate && !ParticleInfo.IsValid())
				{
					NewParticleActor->ParticleSystemTemplate = ParticleSystemTemplate;
					NewParticleActor->ParticleInfo.ParticleSystem = ParticleSystemTemplate;
					NewParticleActor->ParticleInfo.ParticleSystemType = EParticleSystemType::Legacy;
					NewParticleActor->ParticleInfo.DestroyDelay = 1.0f; // Force 1 second
				}
				
				// Initialize legacy settings for new actor
				NewParticleActor->InitializeLegacySettings();
				
				// Spawn and activate the particle system on new actor
				NewParticleActor->SpawnedParticleInfo = NewParticleActor->SpawnParticleSystem(MyTarget, Parameters);
				if (NewParticleActor->SpawnedParticleInfo.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("Particle system spawned and activating..."));
					NewParticleActor->SpawnedParticleInfo.Activate();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to spawn particle system!"));
				}
				
				// Set up automatic destruction timer (1 second)
				float ActorDestroyDelay = 1.0f;
				UE_LOG(LogTemp, Warning, TEXT("Setting destroy timer for %f seconds"), ActorDestroyDelay);
				World->GetTimerManager().SetTimer(
					NewParticleActor->DestroyTimerHandle,
					NewParticleActor,
					&AGameplayCueNotify_ParticleActor::DestroyParticleActor,
					ActorDestroyDelay,
					false
				);
				
				return true;
			}
		}
	}

	return false;
}

void AGameplayCueNotify_ParticleActor::DestroyParticleActor()
{
	// Clean up spawned particle components
	if (SpawnedParticleInfo.IsValid())
	{
		SpawnedParticleInfo.DestroyComponent();
	}

	Destroy();
}

FCrossviewHunter_SpawnedParticleInfo AGameplayCueNotify_ParticleActor::SpawnParticleSystem(AActor* TargetActor, const FGameplayCueParameters& Parameters)
{
	// Create a modifiable copy of ParticleInfo to allow runtime rotation adjustments
	FCrossviewHunter_ParticleInfo ModifiedParticleInfo = ParticleInfo;
	
	// Apply custom rotation logic here
	if (TargetActor)
	{
		FRotator WeaponRotation = TargetActor->GetActorRotation();
		
		// 270도 회전
		ModifiedParticleInfo.RotationOffset = FRotator(270, 0, 0);
		
		UE_LOG(LogTemp, Warning, TEXT("=== Weapon Rotation: %s ==="), *WeaponRotation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("=== Applied RotationOffset: %s ==="), *ModifiedParticleInfo.RotationOffset.ToString());
	}
	
	return SpawnParticleSystemStatic(ModifiedParticleInfo, TargetActor, Parameters);
}

FCrossviewHunter_SpawnedParticleInfo AGameplayCueNotify_ParticleActor::SpawnParticleSystemStatic(const FCrossviewHunter_ParticleInfo& InParticleInfo, AActor* TargetActor, const FGameplayCueParameters& Parameters)
{
	FCrossviewHunter_SpawnedParticleInfo Result;
	
	if (!InParticleInfo.IsValid() || !TargetActor)
	{
		return Result;
	}

	UWorld* World = TargetActor->GetWorld();
	if (!World)
	{
		return Result;
	}

	// Determine which system to use
	EParticleSystemType SystemToUse;
	if (!InParticleInfo.GetEffectiveParticleSystem(SystemToUse))
	{
		UE_LOG(LogTemp, Error, TEXT("No effective particle system found!"));
		return Result;
	}

	UE_LOG(LogTemp, Warning, TEXT("Using particle system type: %d"), (int32)SystemToUse);

	// Calculate spawn transform
	FVector SpawnLocation = Parameters.Location.IsNearlyZero() ? TargetActor->GetActorLocation() : FVector(Parameters.Location);
	FRotator SpawnRotation = Parameters.Normal.IsNearlyZero() ? TargetActor->GetActorRotation() : FVector(Parameters.Normal).Rotation();
	
	UE_LOG(LogTemp, Warning, TEXT("Original spawn location: %s"), *SpawnLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Original spawn rotation: %s"), *SpawnRotation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ParticleInfo RotationOffset: %s"), *InParticleInfo.RotationOffset.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ParticleInfo LocationOffset: %s"), *InParticleInfo.LocationOffset.ToString());
	
	// Apply offsets from particle info
	SpawnLocation += InParticleInfo.LocationOffset;
	SpawnRotation += InParticleInfo.RotationOffset;
	
	UE_LOG(LogTemp, Warning, TEXT("Final spawn location: %s"), *SpawnLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Final spawn rotation: %s"), *SpawnRotation.ToString());

	// Spawn the appropriate system
	switch (SystemToUse)
	{
		case EParticleSystemType::Niagara:
			if (InParticleInfo.NiagaraSystem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn Niagara system: %s"), *InParticleInfo.NiagaraSystem->GetName());
				
				Result.NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					World,
					InParticleInfo.NiagaraSystem,
					SpawnLocation,
					SpawnRotation,
					InParticleInfo.Scale,
					!InParticleInfo.bAutoDestroy
				);
				
				if (Result.NiagaraComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("Niagara component created successfully!"));
					Result.SpawnedSystemType = EParticleSystemType::Niagara;
					
					// Handle attachment
					if (InParticleInfo.bAttachToTarget)
					{
						USceneComponent* AttachComponent = TargetActor->GetRootComponent();
						if (InParticleInfo.AttachPointName != NAME_None)
						{
							if (UMeshComponent* MeshComp = TargetActor->FindComponentByClass<UMeshComponent>())
							{
								AttachComponent = MeshComp;
							}
						}
						Result.NiagaraComponent->AttachToComponent(AttachComponent, FAttachmentTransformRules::KeepWorldTransform, InParticleInfo.AttachPointName);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create Niagara component!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Niagara system is NULL!"));
			}
			break;

		case EParticleSystemType::Legacy:
			if (InParticleInfo.ParticleSystem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn Legacy particle system: %s"), *InParticleInfo.ParticleSystem->GetName());
				
				Result.ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
					World,
					InParticleInfo.ParticleSystem,
					SpawnLocation,
					SpawnRotation,
					InParticleInfo.Scale,
					!InParticleInfo.bAutoDestroy
				);
				
				if (Result.ParticleComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("Legacy particle component created successfully!"));
					Result.SpawnedSystemType = EParticleSystemType::Legacy;
					
					// Handle attachment
					if (InParticleInfo.bAttachToTarget)
					{
						USceneComponent* AttachComponent = TargetActor->GetRootComponent();
						if (InParticleInfo.AttachPointName != NAME_None)
						{
							if (UMeshComponent* MeshComp = TargetActor->FindComponentByClass<UMeshComponent>())
							{
								AttachComponent = MeshComp;
							}
						}
						Result.ParticleComponent->AttachToComponent(AttachComponent, FAttachmentTransformRules::KeepWorldTransform, InParticleInfo.AttachPointName);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create Legacy particle component!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Legacy particle system is NULL!"));
			}
			break;

		default:
			break;
	}

	return Result;
}

void AGameplayCueNotify_ParticleActor::InitializeLegacySettings()
{
	// If ParticleInfo is not configured but we have legacy settings, migrate them
	if (!ParticleInfo.IsValid() && ParticleSystemTemplate)
	{
		ParticleInfo.ParticleSystem = ParticleSystemTemplate;
		ParticleInfo.ParticleSystemType = EParticleSystemType::Legacy;
		ParticleInfo.bAutoDestroy = true;
		ParticleInfo.DestroyDelay = DestroyDelay;
	}
}

FTransform AGameplayCueNotify_ParticleActor::GetParticleSpawnTransform(AActor* TargetActor, const FGameplayCueParameters& Parameters, const FCrossviewHunter_ParticleInfo& InParticleInfo)
{
	FVector SpawnLocation = Parameters.Location.IsNearlyZero() ? TargetActor->GetActorLocation() : FVector(Parameters.Location);
	FRotator SpawnRotation = Parameters.Normal.IsNearlyZero() ? TargetActor->GetActorRotation() : FVector(Parameters.Normal).Rotation();
	
	// Apply offsets
	SpawnLocation += InParticleInfo.LocationOffset;
	SpawnRotation += InParticleInfo.RotationOffset;
	
	return FTransform(SpawnRotation, SpawnLocation, InParticleInfo.Scale);
}