// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CHMonsterSpawnPointRow.generated.h"

/**
 * Monster Spawn Point 정보를 담는 DataTable Row 구조체
 * Phase별로 여러 개의 Spawn Point를 관리
 */
USTRUCT(BlueprintType)
struct CROSSVIEWHUNTER_API FCHMonsterSpawnPointRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FName SpawnPointID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 PhaseNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FName MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 SpawnCount = 1;

	bool bIsInDataTable = true;
};
