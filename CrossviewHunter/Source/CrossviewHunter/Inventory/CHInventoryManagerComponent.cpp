// Fill out your copyright notice in the Description page of Project Settings.


#include "CHInventoryManagerComponent.h"

#include "Character/LyraCharacter.h"
#include "Player/LyraPlayerState.h"
#include "System/CHGameInstance.h"

UCHInventoryManagerComponent::UCHInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UCHInventoryManagerComponent::AddItemInstance(ULyraInventoryItemInstance* ItemInstance, int32 StackCount)
{
	 Super::AddItemInstance(ItemInstance, StackCount);
	//
	// const FUniqueNetIdRepl& PlayerUniqueId = GetOwner<ALyraCharacter>()->GetPlayerState<ALyraPlayerState>()->GetUniqueId();
	//
	// if (PlayerUniqueId.IsValid())
	// {
	// 	FString PlayerID = PlayerUniqueId.ToString();
	// 	//GetWorld()->GetGameInstance<UCHGameInstance>()->SaveInventoryList(InventoryList.Entries, PlayerID);
	// }
}

void UCHInventoryManagerComponent::RemoveItemInstance(ULyraInventoryItemInstance* ItemInstance)
{
	Super::RemoveItemInstance(ItemInstance);
}

