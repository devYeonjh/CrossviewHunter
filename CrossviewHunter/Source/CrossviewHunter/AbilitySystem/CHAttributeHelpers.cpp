// Fill out your copyright notice in the Description page of Project Settings.


#include "CHAttributeHelpers.h"
#include "AbilitySystem/Attributes/CHStatSet.h"



void UCHAttributeHelpers::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	StatDataTable = nullptr;
	StatAttributeMap.Empty();

	InitializeDataTable();
}

void UCHAttributeHelpers::Deinitialize()
{
	Super::Deinitialize();
}

FGameplayAttribute UCHAttributeHelpers::GetAttributeForID(const ECHStatID ID)
{
	FGameplayAttribute ResultAttribute = StatAttributeMap.FindRef(ID);
	return ResultAttribute;
}


void UCHAttributeHelpers::InitializeDataTable()
{
	StatDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/CrossviewHunter/Core/DT_StatData.DT_StatData"));
		
	StatDataTable->ForeachRow<FCHStatData>(FString("FCHStatData"),
		[this](const FName& Key, const FCHStatData& Value)
		{
			StatAttributeMap.Add(Value.AttributeID, Value.AttributeName);
		}
	);
}
