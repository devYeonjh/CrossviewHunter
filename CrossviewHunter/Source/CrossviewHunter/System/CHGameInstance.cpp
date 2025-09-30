// Fill out your copyright notice in the Description page of Project Settings.


#include "CHGameInstance.h"
#include "CHSaveGame.h"
#include "Kismet/GameplayStatics.h"


void UCHGameInstance::InitAttributeSet(UAbilitySystemComponent* AbilitySystemComponent, FName GroupName, int32 Level, bool bInitialInit)
{
	AttributeSetInitter.InitAttributeSetDefaults(AbilitySystemComponent, GroupName, Level, bInitialInit);
	UE_LOG(LogTemp, Log, TEXT("InitAttributeSet: GroupName=%s, Level=%d, bInitialInit=%d"), *GroupName.ToString(), Level, bInitialInit);
}

void UCHGameInstance::UpdateInventoryList(const TArray<FLyraInventoryEntry>& List, const FUniqueNetIdRepl& PlayerId)
{
	if (CachedPlayerDataMap.Contains(PlayerId))
	{
		CachedPlayerDataMap[PlayerId].InventoryList.Empty();
		CachedPlayerDataMap[PlayerId].InventoryList.Append(List);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryList: PlayerId=%s not found"), *PlayerId.ToString());
	}
}

TArray<FLyraInventoryEntry> UCHGameInstance::LoadInventoryList(const FUniqueNetIdRepl& PlayerId)
{
	if (CachedPlayerDataMap.Contains(PlayerId))
	{
		return CachedPlayerDataMap[PlayerId].InventoryList;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadInventoryList: PlayerId=%s not found"), *PlayerId.ToString());
		return TArray<FLyraInventoryEntry>();
	}
}

void UCHGameInstance::SavePlayerData(const FUniqueNetIdRepl& PlayerId)
{
	UCHSaveGame* SaveGame = NewObject<UCHSaveGame>();

}

bool UCHGameInstance::LoadPlayerData(const FUniqueNetIdRepl& PlayerId, FPlayerSaveData& OutData)
{
	// 시작 시 Save Data에서 Data 로드
	// if (UCHSaveGame* SaveGame)
	// {
	// 	if (FPlayerSaveData* SavedData = SaveGame->PlayerDataMap.Find(PlayerId))
	// 	{
	// 		OutData = *SavedData;
	// 		CachedPlayerDataMap.Add(PlayerId, OutData); // 캐시에 저장                                                                  
	// 		return true;
	// 	}
	// }

	return false;
}

void UCHGameInstance::Init()
{
	Super::Init();
	AttributeSetInitter.PreloadAttributeSetData(AttributeSetCurveData);
}

void UCHGameInstance::Shutdown()
{
	Super::Shutdown();
}
