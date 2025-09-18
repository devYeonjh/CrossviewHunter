// Fill out your copyright notice in the Description page of Project Settings.

#include "CHItemDataTableRows.h"
#include "Engine/DataTable.h"
#include "Utility/EnumHelpers.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UCHOptionPool::UCHOptionPool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PoolID(ECHOptionPoolID::pool_200001)
	, LastPoolID(ECHOptionPoolID::pool_200001)
{
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("/Game/CrossviewHunter/Data/DT_ItemOptionDetail.DT_ItemOptionDetail"));
	if (DataTableObject.Succeeded())
	{
		OptionDataTable = DataTableObject.Object;
	}
}

FPrimaryAssetId UCHOptionPool::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType("OptionPool"), UEnumHelpers::GetEnumFName(PoolID));
}



#if WITH_EDITOR
// 에디터에서 PoolID 변경 시 호출
void UCHOptionPool::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		
		// PoolID가 변경된 경우
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UCHOptionPool, PoolID))
		{
			if (PoolID != LastPoolID)
			{
				LoadMatchingOptionsFromDataTable();
				LastPoolID = PoolID;
			}
		}
		// DataTable이 변경된 경우
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(UCHOptionPool, OptionDataTable))
		{
			LoadMatchingOptionsFromDataTable();
		}
	}
}


void UCHOptionPool::RefreshOptionDetails()
{
	LoadMatchingOptionsFromDataTable();
	
	// 에디터 새로고침
	if (GEditor)
	{
		GEditor->RedrawAllViewports();
	}
}

TMap<ECHStatID, int32> UCHOptionPool::GetRandomOptions(int32 Num)
{
	TMap<ECHStatID, int32> Result;
	int32 WeightSum = 0;
	for (FCHItemOptionDetailRow Option : OptionDetails)
	{
		WeightSum += Option.Weight;
	}
	for (int32 i = 0; i < Num; i++)
	{
		int32 RandomWeight = FMath::RandRange(1, WeightSum);
		for (FCHItemOptionDetailRow Option : OptionDetails)
		{
			if (RandomWeight <= Option.Weight)
			{
				int32 RandomValue = FMath::RandRange(Option.MinValue, Option.MaxValue);
				Result.Add(Option.StatID, RandomValue);
				break;
			}
			RandomWeight -= Option.Weight;
		}
	}

	return Result;
}
#endif

void UCHOptionPool::LoadMatchingOptionsFromDataTable()
{
	// 기존 데이터 클리어
	OptionDetails.Empty();
	
	// DataTable 로드
	UDataTable* DataTablePtr = OptionDataTable;
	if (!DataTablePtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OptionDataTable is not set or failed to load"));
		return;
	}
	
	// DataTable이 올바른 구조체인지 확인
	if (DataTablePtr->GetRowStruct() != FCHItemOptionDetailRow::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable has wrong row structure. Expected FCHItemOptionDetailRow"));
		return;
	}
	
	// 모든 행을 순회하며 일치하는 PoolID 찾기
	TArray<FName> RowNames = DataTablePtr->GetRowNames();
	int32 MatchedCount = 0;
	
	for (const FName& RowName : RowNames)
	{
		FCHItemOptionDetailRow* OptionRow = DataTablePtr->FindRow<FCHItemOptionDetailRow>(
			RowName, TEXT("UCHOptionPool::LoadMatchingOptionsFromDataTable"));
		
		if (OptionRow && OptionRow->OptionPoolID == PoolID)
		{
			OptionDetails.Add(*OptionRow);
			MatchedCount++;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Loaded %d options for PoolID: %d"), MatchedCount, (int32)PoolID);
	
#if WITH_EDITOR
	// 에디터에서 프로퍼티 새로고침
	if (GEditor)
	{
		FPropertyChangedEvent PropertyEvent(FindFieldChecked<FProperty>(GetClass(), TEXT("OptionDetails")));
		PostEditChangeProperty(PropertyEvent);
	}
#endif
}
