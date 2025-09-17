// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "System/GameplayTagStack.h"
#include "Templates/SubclassOf.h"

#include "LyraInventoryItemInstance.generated.h"

#define UE_API LYRAGAME_API

class FLifetimeProperty;

class ULyraInventoryItemDefinition;
class ULyraInventoryItemFragment;
struct FFrame;
struct FGameplayTag;

/**
 * ULyraInventoryItemInstance
 */
UCLASS(BlueprintType)
class UE_API ULyraInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lyra|Inventory")
	bool bIsLocked = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Lyra|Inventory")
	bool GetItemState()
	{
		return bIsLocked;
	}

	UFUNCTION(BlueprintCallable, Category = "Lyra|Inventory")
	void SetItemState(bool ItemState)
	{
		bIsLocked = ItemState;
	}



	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	TSubclassOf<ULyraInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	TObjectPtr<ULyraInventoryItemDefinition> GetItemDefInstance() const
	{
		return ItemDefInstance;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	//Item Definition에서 ID 가져오기
	UFUNCTION(BlueprintCallable, Category=Inventory)
	FName GetItemID();

protected:
#if UE_WITH_IRIS
	/** Register all replication fragments */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS

	void SetItemDef(TSubclassOf<ULyraInventoryItemDefinition> InDef);

	void SetItemDefInstance(TObjectPtr<ULyraInventoryItemDefinition> InDefInstance);

	friend struct FLyraInventoryList;
	
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	TObjectPtr<ULyraInventoryItemDefinition> ItemDefInstance;
};

#undef UE_API