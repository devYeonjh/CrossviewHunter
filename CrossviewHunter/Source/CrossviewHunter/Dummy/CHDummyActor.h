// Fill out your copyright notice in the Description page of Project Settings.

// 이 파일은 x157/Lyra-ActorWithAbilities 레포지토리를 참고하여 작성되었습니다.
// 참고 소스: https://github.com/x157/Lyra-ActorWithAbilities/tree/main

#pragma once

#include "CoreMinimal.h"
#include "Actor/CHActorWithAbilitiesAndHealth.h"
#include "CHDummyActor.generated.h"

/**
 * Dummy Actor
 *
 * 어빌리티와 체력을 가진 불멸의 액터입니다.
 *
 * 체력이 절대 1 아래로 떨어지지 않습니다.
 */
UCLASS()
class CROSSVIEWHUNTER_API ACHDummyActor : public ACHActorWithAbilitiesAndHealth
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	ACHDummyActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ACHActorWithAbilities implementation
	virtual void InitializeAbilitySystem() override;
	//~End of ACHActorWithAbilities implementation

	//~AActor implementation
	virtual void BeginPlay() override;
	//~End of AActor implementation

	/** 액터의 루트 스켈레탈 메시 컴포넌트 가져오기 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return MeshComponent; }

protected:
	/** 루트 컴포넌트는 스켈레탈 메시 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
};
