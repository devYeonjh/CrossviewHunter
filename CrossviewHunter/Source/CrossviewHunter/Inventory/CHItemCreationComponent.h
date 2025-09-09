// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameStateComponent.h"
#include "AbilitySystem/ChAttributeTypes.h"

#include "CHItemCreationComponent.generated.h"

class ULyraInventoryItemFragment;
class ULyraExperienceDefinition;
class ULyraInventoryItemDefinition;
class ULyraInventoryItemInstance;
struct FItemDataTableRow;


UCLASS(Blueprintable, Abstract)
class CROSSVIEWHUNTER_API UCHItemCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

	/** Finds item data in DataTable by ItemID */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	bool FindItemDataByID(int32 ItemID, FItemDataTableRow& OutItemData);
	
	/** Creates item instance based on ItemID from DataTable */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	ULyraInventoryItemInstance* CreateItemInstance(int32 ItemID);

private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);

protected:
	/** DataTable containing item definitions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TObjectPtr<UDataTable> ItemDataTable;
	
	/** Array to track created item instances for testing */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ULyraInventoryItemInstance>> CreatedItems;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ULyraInventoryItemFragment>> TestFragmentList;

	UPROPERTY(EditAnywhere)
	TMap<ECHStatID, float> TestModifiers;
};
