// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/GameStateComponent.h"
#include "CHLootingItemManagerComponent.generated.h"

struct FCHLootGroupEntryRow;
struct FCHLootTableRow;
class ULyraExperienceDefinition;
/**
 * 아이템 랜덤 선택 컴포넌트 (GameStateComponent)
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHLootingItemManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UCHLootingItemManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

protected:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = LootManager)
	TArray<FName> GetLootedItemIDList(TArray<FName> LootTableIDList);
	
private:
	struct FLootGroupData
	{
		FLootGroupData() {}
		~FLootGroupData() {}
		
		TMap<FName, int32> ItemWeightMap;
		TMap<FName, int32> ItemQuantityMap;

		// 아이템 데이터 추가 FCHLootGroupEntryRow 기반
		void AddItemData(const FCHLootGroupEntryRow& Entry);

		// 아이템 ID 랜덤 선택
		FName GetRandomItemID() const;

		// 아이템 ID와 Quantity 랜덤 선택
		FName GetRandomItemIDWithQuantity(int32& OutQuantity) const;
	};
	
	void CacheLootGroupData(TArray<UDataTable*> LootGroupList);
	
	/**
	 * LootTable의 Drop Chance 계산
	 * @return Loot Group ID
	 */
	FName CalcDropChanceLootTable(const FCHLootTableRow& LootTableRow);
	FName GetItemIDFromLootGroup(const FName& LootGroupID);
	
private:
	TArray<TObjectPtr<UDataTable>> CachedLootTables;
	TMap<FName, FLootGroupData> CachedLootGroupData;

	
	
	
};
