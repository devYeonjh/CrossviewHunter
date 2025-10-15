// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CHGameInstance.h"

#include "CHSaveGame.generated.h"

/**
 *  Custom SaveGame 클래스
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHSaveGame : public USaveGame
{
	GENERATED_BODY()

public: 
	UPROPERTY()
	TMap<FUniqueNetIdRepl, FPlayerSaveData> PlayerDataMap;

	UPROPERTY()
	FString SaveSlotName = TEXT("CHPlayerSave");
};