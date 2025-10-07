// Fill out your copyright notice in the Description page of Project Settings.


#include "CHLootingItemManagerComponent.h"

#include "DataTable/CHDataTableManager.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Item/CHLootTableRow.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "System/CHGameInstance.h"

///////////////////////////////////////////////////
/// FLootGroupData Functions
void UCHLootingItemManagerComponent::FLootGroupData::AddItemData(const FCHLootGroupEntryRow& Entry)
{
	if (!ItemWeightMap.Contains(Entry.ItemID))
	{
		ItemWeightMap.Add(Entry.ItemID, Entry.Weight);
	}
	if (!ItemQuantityMap.Contains(Entry.ItemID))
	{
		ItemQuantityMap.Add(Entry.ItemID, Entry.QuantityMin);
	}
}

FName UCHLootingItemManagerComponent::FLootGroupData::GetRandomItemID() const
{
	int WeightSum = 0;
	for (const auto& Pair : ItemWeightMap)
	{
		if (Pair.Value >= 0)
			WeightSum += Pair.Value;
	}

	FName Result = NAME_None;
	int RandomWeight = FMath::RandRange(1, WeightSum);
	for (const auto& Pair : ItemWeightMap)
	{
		if (RandomWeight <= Pair.Value)
		{
			Result = Pair.Key;
			break;
		}
		RandomWeight -= Pair.Value;
	}
	if (Result == NAME_None)
	{
		// Result가 NAME_None인 경우, ItemWeightMap의 첫 번째 아이템 ID를 반환 (오류 처리)
		Result = ItemWeightMap.begin()->Key;
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid item"));
	}
	return Result;
}

FName UCHLootingItemManagerComponent::FLootGroupData::GetRandomItemIDWithQuantity(int32& OutQuantity) const
{
	//@TODO: Implement quantity logic
	return NAME_None;
}


///////////////////////////////////////////////////
/// UCHLootingItemManagerComponent Functions
UCHLootingItemManagerComponent::UCHLootingItemManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCHLootingItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_LowPriority(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void UCHLootingItemManagerComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		if (UGameInstance* GI = GetGameInstance<UCHGameInstance>())
		{
			if (UCHDataTableManager* DataTableManager = GI->GetSubsystem<UCHDataTableManager>())
			{
				TArray<UDataTable*> LootTables = DataTableManager->GetDataTableList(FGameplayTag::RequestGameplayTag("CH.DT.LootTable"));
				if (LootTables.Num() > 0)
				{
					CachedLootTables = LootTables;
				}
				TArray<UDataTable*> LootGroups = DataTableManager->GetDataTableList(FGameplayTag::RequestGameplayTag("CH.DT.LootGroup"));
				if (LootGroups.Num() > 0)
				{
					CacheLootGroupData(LootGroups);
				}
			}
		}
	}
#endif	
}


TArray<FName> UCHLootingItemManagerComponent::GetLootedItemIDList(TArray<FName> LootTableIDList)
{
	TArray<FName> LootedItemIDList = TArray<FName>();

	// 아이디 리스트 순회
	for (const FName& LootTableID : LootTableIDList)
	{
		// 루트 테이블 리스트 순회
		for (const TObjectPtr<UDataTable>& LootTable : CachedLootTables)
		{
			//LootTableID에 해당하는 LootTableRow 찾기
			if (const FCHLootTableRow* LootTableRow = LootTable->FindRow<FCHLootTableRow>(LootTableID, TEXT("GetLootedItemIDList")))
			{
				// Drop Chance에 따라 LootGroupID 반환
				FName LootGroupID = CalcDropChanceLootTable(*LootTableRow);
				if (LootGroupID != NAME_None)
				{
					LootedItemIDList.Add(GetItemIDFromLootGroup(LootGroupID));
				}
				// 해당 LootTable에서 LootTable ID를 찾은 경우 순회 종료(LootTableID는 중복 불가)
				break;
			}
		}
	}

	return LootedItemIDList; 
}


void UCHLootingItemManagerComponent::CacheLootGroupData(TArray<UDataTable*> LootGroupList)
{
	// LootGroupList 순회
	for (UDataTable* LootGroup : LootGroupList)
	{
		// LootGroup이 유효한지 확인
		if (!LootGroup)
			continue;
		
		// LootGroup에서 모든 Row를 가져옴
		TArray<FCHLootGroupEntryRow*> LootGroupEntryRowList;
		LootGroup->GetAllRows<FCHLootGroupEntryRow>(TEXT("GetAllRowsContext"), LootGroupEntryRowList);

		// LootGroupEntryRowList 순회
		for (const FCHLootGroupEntryRow* Entry : LootGroupEntryRowList)
		{
			// CachedLootGroupData에 Entry의 LootGroupID와 Data 매핑
			CachedLootGroupData.FindOrAdd(Entry->LootGroupID).AddItemData(*Entry);
		}
	}
}

FName UCHLootingItemManagerComponent::CalcDropChanceLootTable(const FCHLootTableRow& LootTableRow)
{
	float RandomValue = FMath::FRand();
	if (RandomValue <= LootTableRow.DropChance)
	{
		return LootTableRow.LootGroupID;
	}
	return NAME_None;
}

FName UCHLootingItemManagerComponent::GetItemIDFromLootGroup(const FName& LootGroupID)
{
	if (CachedLootGroupData.Contains(LootGroupID))
	{
		return CachedLootGroupData[LootGroupID].GetRandomItemID();
	}
	return NAME_None;
}
