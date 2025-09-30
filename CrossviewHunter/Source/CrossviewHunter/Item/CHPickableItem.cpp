// Fill out your copyright notice in the Description page of Project Settings.


#include "CHPickableItem.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Async/TaskGraphInterfaces.h"

struct FInteractionQuery;

ACHPickableItem::ACHPickableItem()
{
	//Collect Gameplay Queue가 루트 컴포넌트인 Static Mesh Comp의 값을 참조하여 작동
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);

	static ConstructorHelpers::FObjectFinder<UGameplayAbility> GA_Interaction(TEXT("/ShooterExplorer/Interact/GA_Interaction_Collect.GA_Interaction_Collect"));
	if (GA_Interaction.Succeeded())
	{
		Option.InteractionAbilityToGrant = GA_Interaction.Object.GetClass();
	}
}

void ACHPickableItem::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup ACHPickableItem::GetPickupInventory() const
{
	return InventoryItemInfo;
}

void ACHPickableItem::SetInventoryItemInfo(const TObjectPtr<ULyraInventoryItemInstance>& ItemInstance)
{
	FPickupInstance ItemInfo;
	ItemInfo.Item = ItemInstance;
	InventoryItemInfo.Instances.Add(ItemInfo);
}

void ACHPickableItem::SetInventoryItemInfoWithCount(const ULyraInventoryItemInstance* ItemInstance, int32 Count)
{
	FPickupTemplate ItemInfo;
	ItemInfo.ItemDef = ItemInstance->GetItemDef();
	ItemInfo.StackCount = Count;
	InventoryItemInfo.Templates.Add(ItemInfo);
}

void ACHPickableItem::SetStaticMesh(UStaticMesh* Mesh) const
{
	StaticMeshComp->SetStaticMesh(Mesh);
}

