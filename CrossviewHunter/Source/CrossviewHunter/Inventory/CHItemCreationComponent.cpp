// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemCreationComponent.h"
#include "CHLogChannel.h"

#include "CHItemDefinition.h"
#include "CHItemInstance.h"


UCHItemCreationComponent::UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UCHItemCreationComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCHItemCreationComponent::FindItemDataByID(int32 ItemID, FItemDataTableRow& OutItemData)
{
	return false;
}

ULyraInventoryItemInstance* UCHItemCreationComponent::CreateItemInstance(int32 ItemID)
{
	UCHItemInstance* ItemInstance = NewObject<UCHItemInstance>();
	ItemInstance->SetItemDefinitionData(TestFragmentList, TestModifiers);

	CreatedItems.Add(ItemInstance);

	UE_LOG(LogCH, Log, TEXT("ItemInstance Created"));
	
	return ItemInstance;
}

void UCHItemCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
	
}
