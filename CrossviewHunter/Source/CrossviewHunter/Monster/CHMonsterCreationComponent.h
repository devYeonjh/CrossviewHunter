// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "CHMonsterCreationComponent.generated.h"


class ULyraExperienceDefinition;

UCLASS(BlueprintType, Blueprintable, Abstract)
class CROSSVIEWHUNTER_API UCHMonsterCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UCHMonsterCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

protected:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);
};
