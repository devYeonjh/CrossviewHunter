// Fill out your copyright notice in the Description page of Project Settings.


#include "CHDamageExecution.h"

#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "AbilitySystem/LyraAbilitySourceInterface.h"
#include "AbilitySystem/Attributes/CHStatSet.h"
#include "Engine/World.h"
#include "Teams/LyraTeamSubsystem.h"
#include "CHLogChannel.h"

struct FCHDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
	FGameplayEffectAttributeCaptureDefinition AttackDef;
	FGameplayEffectAttributeCaptureDefinition DefenceDef;
	FGameplayEffectAttributeCaptureDefinition CritRateDef;
	FGameplayEffectAttributeCaptureDefinition ArmorPiercingDamageDef;
	FGameplayEffectAttributeCaptureDefinition CritDamageDef;
	FGameplayEffectAttributeCaptureDefinition HeadDamageDef;
	
	FCHDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(ULyraCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AttackDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		DefenceDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetDefenceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		CritRateDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetCritRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		ArmorPiercingDamageDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetArmorPiercingDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		CritDamageDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetCritDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		HeadDamageDef = FGameplayEffectAttributeCaptureDefinition(UCHStatSet::GetHeadDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FCHDamageStatics& DamageStatics()
{
	static FCHDamageStatics Statics;
	return Statics;
}


UCHDamageExecution::UCHDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPiercingDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().HeadDamageDef);
}

void UCHDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FLyraGameplayEffectContext* TypedContext = FLyraGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypedContext);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// 기본 데미지 계산을 위한 값 세팅
	float BaseDamage = 0.0f;
	float Attack = 0.0f;
	float Defence = 0.0f;
	float CritRate = 0.0f;
	float ArmorPiercingDamage = 0.0f;
	float CritDamage = 0.0f;
	float HeadDamage = 0.0f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluateParameters, Attack);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceDef, EvaluateParameters, Defence);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef, EvaluateParameters, CritRate);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPiercingDamageDef, EvaluateParameters, ArmorPiercingDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvaluateParameters, CritDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HeadDamageDef, EvaluateParameters, HeadDamage);

	BaseDamage = FMath::Max<float>(BaseDamage, 0.0f);
	Attack = FMath::Max<float>(Attack, 0.0f);
	Defence = FMath::Max<float>(Defence, 0.0f);
	CritRate = FMath::Max<float>(CritRate, 0.0f);
	ArmorPiercingDamage = FMath::Max<float>(ArmorPiercingDamage, 0.0f);
	CritDamage = FMath::Max<float>(CritDamage, 1.0f);
	HeadDamage = FMath::Max<float>(HeadDamage, 1.0f);

	
	const AActor* EffectCauser = TypedContext->GetEffectCauser();
	const FHitResult* HitActorResult = TypedContext->GetHitResult();

	AActor* HitActor = nullptr;
	FVector ImpactLocation = FVector::ZeroVector;
	FVector ImpactNormal = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;
	FVector EndTrace = FVector::ZeroVector;

	// hit actor, surface, zone, distance의 계산은 모두 계산에 hit result가 있는지 여부에 의존합니다.
	// 타겟팅되지 않고 직접 추가되는 이펙트들은 항상 hit result 없이 들어오므로
	// 일부 fallback 정보를 기본값으로 사용해야 합니다.
	if (HitActorResult)
	{
		const FHitResult& CurHitResult = *HitActorResult;
		HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if (HitActor)
		{
			ImpactLocation = CurHitResult.ImpactPoint;
			ImpactNormal = CurHitResult.ImpactNormal;
			StartTrace = CurHitResult.TraceStart;
			EndTrace = CurHitResult.TraceEnd;
		}
	}

	// hit result가 없거나 hit result가 실제로 actor를 반환하지 않는 경우를 처리
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!HitActor)
	{
		HitActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
		if (HitActor)
		{
			ImpactLocation = HitActor->GetActorLocation();
		}
	}

	// Apply rules for team damage/self damage/etc...
	float DamageInteractionAllowedMultiplier = 0.0f;
	if (HitActor)
	{
		ULyraTeamSubsystem* TeamSubsystem = HitActor->GetWorld()->GetSubsystem<ULyraTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			DamageInteractionAllowedMultiplier = TeamSubsystem->CanCauseDamage(EffectCauser, HitActor) ? 1.0 : 0.0;
		}
	}

	// Determine distance
	double Distance = WORLD_MAX;

	if (TypedContext->HasOrigin())
	{
		Distance = FVector::Dist(TypedContext->GetOrigin(), ImpactLocation);
	}
	else if (EffectCauser)
	{
		Distance = FVector::Dist(EffectCauser->GetActorLocation(), ImpactLocation);
	}
	else
	{
		UE_LOG(LogCH, Error, TEXT("Damage Calculation cannot deduce a source location for damage coming from %s; Falling back to WORLD_MAX dist!"), *GetPathNameSafe(Spec.Def));
	}
	
	
	// 크리티컬 계산
	float CriticalChance = FMath::FRand();
	CritDamage = CriticalChance <= CritRate ? CritDamage : 1.0f;
	
	float DistanceAttenuation = 1.0f;
	if (const ILyraAbilitySourceInterface* AbilitySource = TypedContext->GetAbilitySource())
	{
		if (const UPhysicalMaterial* PhysMat = TypedContext->GetPhysicalMaterial())
		{
			// 헤드샷 배율 추가 Range Weapon에서 매핑되어있는 태그가 없는경우 1.0f로 반환
			HeadDamage = FMath::IsNearlyEqualByULP(AbilitySource->GetPhysicalMaterialAttenuation(PhysMat, SourceTags, TargetTags),1.0f ) ? 1.0f : HeadDamage;
		}
		// 거리 감쇠 적용
		DistanceAttenuation = AbilitySource->GetDistanceAttenuation(Distance, SourceTags, TargetTags);
	}
	DistanceAttenuation = FMath::Max(DistanceAttenuation, 0.0f);



	// 실제 데미지 계산
	// @TODO: 정확한 데미지 계산 공식 추가
	
	BaseDamage =
		((BaseDamage + FMath::Max<float>(Attack - Defence, 0.0f)) * CritDamage * HeadDamage)+ ArmorPiercingDamage;

	// Clamping is done when damage is converted to -health
	const float DamageDone = FMath::Max(BaseDamage * DistanceAttenuation * DamageInteractionAllowedMultiplier, 0.0f);

	if (DamageDone > 0.0f)
	{
		// Apply a damage modifier, this gets turned into - health on the target
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULyraHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
#endif // #if WITH_SERVER_CODE
}