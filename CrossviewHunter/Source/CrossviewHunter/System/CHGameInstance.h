// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "System/LyraGameInstance.h"
#include "CHGameInstance.generated.h"

class ULyraHealthSet;

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	FUniqueNetIdRepl PlayerUniqueId;

	TArray<FLyraInventoryEntry> InventoryList;

	TArray<FLyraAppliedEquipmentEntry> EquipmentList;

	int32 Level;
};

/**
 * 레벨 변환시에도 사라지지않는 싱글톤 클래스
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHGameInstance : public ULyraGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void InitAttributeSet(UAbilitySystemComponent* AbilitySystemComponent, FName GroupName, int32 Level, bool bInitialInit);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UpdateInventoryList(const TArray<FLyraInventoryEntry>& List, const FUniqueNetIdRepl& PlayerUniqueId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	TArray<FLyraInventoryEntry> LoadInventoryList(const FUniqueNetIdRepl& PlayerId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SavePlayerData(const FUniqueNetIdRepl& PlayerId);

	bool LoadPlayerData(const FUniqueNetIdRepl& PlayerId, FPlayerSaveData& OutData);
	
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
private:
	FAttributeSetInitterDiscreteLevels AttributeSetInitter;

	UPROPERTY()
	TMap<FUniqueNetIdRepl, FPlayerSaveData> CachedPlayerDataMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CH|GameInstance", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UCurveTable>> AttributeSetCurveData;
	
};
