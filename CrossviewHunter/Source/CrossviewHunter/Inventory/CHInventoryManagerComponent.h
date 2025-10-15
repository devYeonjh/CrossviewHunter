// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "CHInventoryManagerComponent.generated.h"


UCLASS()
class CROSSVIEWHUNTER_API UCHInventoryManagerComponent : public ULyraInventoryManagerComponent
{
	GENERATED_BODY()

	UCHInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//~ULyraInventoryManagerComponent interface
	virtual void AddItemInstance(ULyraInventoryItemInstance* ItemInstance, int32 StackCount = 1) override;
	virtual void RemoveItemInstance(ULyraInventoryItemInstance* ItemInstance) override;

	TArray<FLyraInventoryEntry>& GetAddableInventoryList();

private:
	


};
