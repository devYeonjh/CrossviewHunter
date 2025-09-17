// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interaction/IInteractableTarget.h"
#include "Interaction/InteractionOption.h"
#include "Inventory/IPickupable.h"

#include "CHPickableItem.generated.h"

class UObject;
class ULyraInventoryItemInstance;
class UStaticMeshComponent;
struct FInteractionQuery;

UCLASS()
class CROSSVIEWHUNTER_API ACHPickableItem : public AActor, public IInteractableTarget, public IPickupable
{
	GENERATED_BODY()

public:
	ACHPickableItem();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;
	
	void SetInventoryItemInfo(const TObjectPtr<ULyraInventoryItemInstance>& ItemInstance);
	void SetInventoryItemInfoWithCount(const ULyraInventoryItemInstance* ItemInstance, int32 Count);
	void SetStaticMesh(UStaticMesh* Mesh) const;

protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

	UPROPERTY(EditAnywhere)
	FInventoryPickup InventoryItemInfo;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
};
