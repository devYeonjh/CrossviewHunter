// Fill out your copyright notice in the Description page of Project Settings.


#include "CHPickableItem.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Async/TaskGraphInterfaces.h"

struct FInteractionQuery;

ACHPickableItem::ACHPickableItem()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

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

void ACHPickableItem::SetInventoryItemInfo(const ULyraInventoryItemInstance* ItemInstance)
{
	FPickupTemplate ItemInfo;
	ItemInfo.ItemDef = ItemInstance->GetItemDef();
	ItemInfo.StackCount = 1;
	InventoryItemInfo.Templates.Add(ItemInfo);
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

