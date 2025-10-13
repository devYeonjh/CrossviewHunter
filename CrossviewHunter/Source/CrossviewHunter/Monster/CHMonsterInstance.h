// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CHMonsterDataTableRow.h"
#include "CHMonsterInstance.generated.h"

class ALyraTaggedActor;
/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHMonsterInstance : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	FName MonsterID;

	UPROPERTY()
	TSubclassOf<ALyraTaggedActor> MonsterCosmetic;

	UPROPERTY()
	FCHMonsterDataRow MonsterData;

	
};
