// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_CHEquipmentInfo.h"

void UInventoryFragment_CHEquipmentInfo::SetModifiers(const TMap<ECHStatID, float>& InModifiers)
{
	Modifiers = InModifiers;
}

TMap<ECHStatID, float> UInventoryFragment_CHEquipmentInfo::GetModifiers() const
{
	return Modifiers;
}
