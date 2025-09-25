// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LyraPlayerSpawningManagerComponent.h"

#include "CHSpawningManagerComponent.generated.h"

class AActor;
class AController;
class ALyraPlayerStart;
class UObject;


/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHSpawningManagerComponent : public ULyraPlayerSpawningManagerComponent
{
	GENERATED_BODY()

public:

	UCHSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<ALyraPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;

protected:

};
