// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemDefinition.h"

void UCHItemDefinition::SetItemData(const FItemDataTableRow& DataTableRow)
{
	DisplayName = DataTableRow.ItemName;
	//Fragments = DataTableRow.Fragments;
}
