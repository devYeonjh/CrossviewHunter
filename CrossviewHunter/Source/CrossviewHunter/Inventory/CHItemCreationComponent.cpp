// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemCreationComponent.h"
#include "CHLogChannel.h"
#include "CHItemDefinition.h"
#include "CHItemInstance.h"
#include "Equipment/CHEquipmentInstance.h"
#include "Character/LyraCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryFragment_CHEquipmentInfo.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Inventory/LyraInventoryItemDefinition.h"


UCHItemCreationComponent::UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UCHItemCreationComponent::BeginPlay()
{
	Super::BeginPlay();
}

FItemDataTableRow& UCHItemCreationComponent::FindItemDataByID(FName ItemID) const
{
	FItemDataTableRow* OutItemDataRow = nullptr;
	UDataTableFunctionLibrary::Generic_GetDataTableRowFromName(ItemDataTable, ItemID, OutItemDataRow);

	return *OutItemDataRow;
}


ULyraInventoryItemInstance* UCHItemCreationComponent::CreateItemInstance(FName ItemID)
{
	TObjectPtr<UCHItemInstance> ItemInstance = NewObject<UCHItemInstance>();
	TObjectPtr<UCHItemDefinition> ItemDefinition = NewObject<UCHItemDefinition>();

	const FItemDataTableRow& ItemDataRow = FindItemDataByID(ItemID);
	ItemDefinition->SetItemData(ItemDataRow);

	
	CreatedItems.Add(ItemInstance);

	UE_LOG(LogCH, Log, TEXT("ItemInstance Created"));
	return ItemInstance;
}

void UCHItemCreationComponent::LogCreatedItem()
{
	for (ULyraInventoryItemInstance* Item: CreatedItems)
	{
		UE_LOG(LogCH, Log, TEXT("Actor Name: %s"), *Item->GetName());
	}
}

void UCHItemCreationComponent::EquipOn()
{
	ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UCHEquipmentInstance* EquipmentInstance = NewObject<UCHEquipmentInstance>();

	const UInventoryFragment_CHEquipmentInfo* Fragment = CreatedItems[0]->FindFragmentByClass<UInventoryFragment_CHEquipmentInfo>();
	
	
}

void UCHItemCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
	
}

