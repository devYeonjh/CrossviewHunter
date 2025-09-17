// Fill out your copyright notice in the Description page of Project Settings.


#include "CHQuickBarComponent.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/CHEquipmentManagerComponent.h"
#include "Item/CHItemInstance.h"
#include "Equipment/LyraEquipmentInstance.h"

void UCHQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (ULyraInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UInventoryFragment_EquippableItem>())
		{
			if (UCHEquipmentManagerComponent* EquipmentManager = Cast<UCHEquipmentManagerComponent>(FindEquipmentManager()))
			{
				if (UCHItemInstance* ItemInstance =  Cast<UCHItemInstance>(SlotItem))
				{
					EquippedItem = EquipmentManager->EquipItemInstance(ItemInstance);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
				else
				{
					Super::EquipItemInSlot();
				}
			}
			else
			{
				Super::EquipItemInSlot();
			}
		}
	}
}

void UCHQuickBarComponent::UnequipItemInSlot()
{
	if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (UCHEquipmentManagerComponent* CHManager = Cast<UCHEquipmentManagerComponent>(EquipmentManager))
		{
			if (EquippedItem != nullptr)
			{
				CHManager->UnequipItem(EquippedItem);
				EquippedItem = nullptr;
			}
		}
		else
		{
			Super::UnequipItemInSlot();
		}
	}
}
