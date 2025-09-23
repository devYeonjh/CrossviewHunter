// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#pragma once

#include "CoreMinimal.h"
#include "CHActorWithAbilities.h"
#include "CHActorWithAbilitiesAndHealth.generated.h"

class ULyraCombatSet;
class ULyraHealthSet;
class ULyraHealthComponent;

/**
 * 어빌리티와 체력을 가진 액터
 *
 * 액터에 Health Set과 Health Component를 추가하여
 * 데미지, 힐, 사망 등을 처리할 수 있습니다.
 */
UCLASS(Blueprintable)
class CROSSVIEWHUNTER_API ACHActorWithAbilitiesAndHealth : public ACHActorWithAbilities
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	ACHActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ACHActorWithAbilities implementation
	virtual void InitializeAbilitySystem() override;
	virtual void UninitializeAbilitySystem() override;
	//~End of ACHActorWithAbilities implementation

protected:
	/**
	 * Lyra "Combat Set"은 Lyra 힐링 및 데미지 이펙트 계산에 필요합니다.
	 *
	 * 이 세트는 기본 힐링 또는 데미지 값을 정의합니다.
	 *
	 * 선택사항이어야 하지만(기본값은 모두 0), 존재하지 않으면
	 * Lyra가 경고 로그 메시지를 자주 출력합니다.
	 *
	 * 기본적으로 이러한 속성들에 대해 모두 0을 사용합니다.
	 *
	 * 필요에 따라 오버라이드하세요.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CH|Ability")
	TObjectPtr<ULyraCombatSet> CombatSet;

	/**
	 * Lyra "Health Set"은 Lyra 힐링 및 데미지 이펙트 계산에 필요합니다.
	 *
	 * 체력, 최대 체력, 들어오는 힐링, 들어오는 데미지를 정의합니다.
	 *
	 * 사용자 정의 속성을 추가하려면 이 클래스를 상속받아
	 * 자신만의 클래스를 만드는 것을 권장합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CH|Ability")
	TObjectPtr<ULyraHealthSet> HealthSet;

	/**
	 * Lyra "Health Component"는 HealthSet을 위한 편의 인터페이스입니다
	 *
	 * HealthSet이 필요하며 해당 이벤트 핸들러에 연결됩니다.
	 * 예를 들어 블루프린트에서 이러한 이벤트에 쉽게 응답할 수 있게 해줍니다.
	 *
	 * 이 컴포넌트는 또한 네트워크 복제를 통해 액터의 사망을
	 * 처리하는 역할을 담당하는 것 같습니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CH|Ability")
	TObjectPtr<ULyraHealthComponent> HealthComponent;
};
