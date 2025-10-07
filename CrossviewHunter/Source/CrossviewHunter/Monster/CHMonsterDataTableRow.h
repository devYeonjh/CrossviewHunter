// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHMonsterTypes.h"

#include "CHMonsterDataTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FCHMonsterDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	FName MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	FText MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	ECHMonsterGrade MonsterGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	int32 EXPReward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TArray<FName> LootTableIDList;
	
	
};
