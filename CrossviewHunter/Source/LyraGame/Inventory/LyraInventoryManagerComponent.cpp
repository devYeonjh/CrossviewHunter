// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "LyraInventoryItemDefinition.h"
#include "InventoryFragment_Stackable.h"
#include "LyraInventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Lyra_Inventory_Message_StackChanged, "Lyra.Inventory.Message.StackChanged");

//////////////////////////////////////////////////////////////////////
// FLyraInventoryEntry

FString FLyraInventoryEntry::GetDebugString() const
{
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FLyraInventoryList

void FLyraInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FLyraInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FLyraInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FLyraInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FLyraInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FLyraInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FLyraInventoryList::BroadcastChangeMessage(FLyraInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FLyraInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
}

ULyraInventoryItemInstance* FLyraInventoryList::AddEntry(
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef, int32 StackCount)
{
	ULyraInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	for (FLyraInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance && Entry.Instance->GetItemDef() == ItemDef)
		{
			// 🔹 Stackable 프래그먼트 확인
			if (const UInventoryFragment_Stackable* StackableFrag = Entry.Instance->FindFragmentByClass<UInventoryFragment_Stackable>())
			{
				// 현재 수량 + 추가 수량
				int32 NewCount = Entry.StackCount + StackCount;

				if (NewCount > StackableFrag->MaxStackSize)
				{
					// 최대치까지만 채움
					int32 Overflow = NewCount - StackableFrag->MaxStackSize;
					Entry.StackCount = StackableFrag->MaxStackSize;
					MarkItemDirty(Entry);

					// 남은 개수는 새로 생성 루틴으로 넘김
					StackCount = Overflow;
					continue;// 다음 Entry 검사 (혹은 for문 끝나고 새 Entry 생성)
				}
				else
				{
					// 아직 최대치 이하 → 그냥 누적
					Entry.StackCount = NewCount;
					MarkItemDirty(Entry);
					return Entry.Instance;
				}
			}
		}
	}
	
	FLyraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULyraInventoryItemInstance>(
		OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172

	NewEntry.Instance->SetItemDef(ItemDef);
	
	for (ULyraInventoryItemFragment* Fragment : GetDefault<ULyraInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const ULyraInventoryItemDefinition* ItemCDO = GetDefault<ULyraInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

TArray<TObjectPtr<ULyraInventoryItemInstance>> FLyraInventoryList::AddEntry(ULyraInventoryItemInstance* Instance, int32 StackCount)
{
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Result;
	
	check(Instance != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	if (const UInventoryFragment_Stackable* StackableFrag = Instance->FindFragmentByClass<UInventoryFragment_Stackable>())
	{
		Result.Append(AddStack(Instance,Instance->GetItemID(), StackCount, StackableFrag->MaxStackSize));
		return Result;
	}
	FLyraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = Instance;

	for (ULyraInventoryItemFragment* Fragment : Instance->GetItemDefInstance()->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;

	MarkItemDirty(NewEntry);
	Result.Add(NewEntry.Instance);
	
	return Result;
}

TArray<TObjectPtr<ULyraInventoryItemInstance>> FLyraInventoryList::AddStack(ULyraInventoryItemInstance* Instance,
	FName ItemID, int32 StackCount, int32 MaxCount)
{
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Result;

	
	int32 CurrentCount = StackCount;
	for (FLyraInventoryEntry& Entry : Entries)
	{
		//같은 아이템이 있는 경우
		if (Entry.Instance && Entry.Instance->GetItemID() == ItemID)
		{
			// 현재 수량 + 추가 수량
			CurrentCount = Entry.StackCount + CurrentCount;

			if (CurrentCount > MaxCount)
			{
				// 최대치까지만 채움
				Entry.StackCount = MaxCount;

				Result.Add(Entry.Instance);
				MarkItemDirty(Entry);
				
				// 남은 개수는 새로 생성 루틴으로 넘김
				CurrentCount = CurrentCount - MaxCount;
			}
			else
			{
				// 아직 최대치 이하 → 그냥 누적
				Entry.StackCount = CurrentCount;
				Result.Add(Entry.Instance);
				MarkItemDirty(Entry);

				CurrentCount = 0;
				break;
			}
		}
	}
	// 남은 갯수가 있는경우 추가
	while (CurrentCount > 0)
	{
		FLyraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Instance = Instance;

		if (CurrentCount > MaxCount)
		{
			NewEntry.StackCount = MaxCount;
			CurrentCount = CurrentCount - MaxCount;
		}
		else
		{
			NewEntry.StackCount = CurrentCount;
			CurrentCount = 0;
		}

		Result.Add(NewEntry.Instance);
		MarkItemDirty(NewEntry);
	}

	return Result;
}

void FLyraInventoryList::RemoveEntry(ULyraInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<ULyraInventoryItemInstance*> FLyraInventoryList::GetAllItems() const
{
	TArray<ULyraInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FLyraInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FLyraInventoryList::SortByDefinition()
{
	Entries.Sort([](const FLyraInventoryEntry& A, const FLyraInventoryEntry& B)
	{
		auto GetItemDef = [](const FLyraInventoryEntry& Entry)
		{
			return Entry.Instance ? Entry.Instance->GetItemDef() : nullptr;
		};

		auto HasStackable = [](const FLyraInventoryEntry& Entry)
		{
			return (Entry.Instance && Entry.Instance->FindFragmentByClass<UInventoryFragment_Stackable>() != nullptr);
		};

		TSubclassOf<ULyraInventoryItemDefinition> DefA = GetItemDef(A);
		TSubclassOf<ULyraInventoryItemDefinition> DefB = GetItemDef(B);

		// 1. Stackable 없는 아이템 먼저, Stackable 있는 아이템 뒤로
		bool A_HasStackable = HasStackable(A);
		bool B_HasStackable = HasStackable(B);

		if (A_HasStackable != B_HasStackable)
		{
			return !A_HasStackable; // A가 비-스택이면 A 먼저
		}

		// 2. 같은 정의라면 StackCount 많은 순
		if (DefA == DefB)
		{
			return A.StackCount > B.StackCount;
		}

		// 3. 그 외에는 정렬 안 함 (원래 순서 유지)
		return false;
	});
}

//////////////////////////////////////////////////////////////////////
// ULyraInventoryManagerComponent

ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void ULyraInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool ULyraInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

ULyraInventoryItemInstance* ULyraInventoryManagerComponent::AddItemDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef, int32 StackCount)
{
	ULyraInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void ULyraInventoryManagerComponent::AddItemInstance(ULyraInventoryItemInstance* ItemInstance, int32 StackCount)
{
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Results = InventoryList.AddEntry(ItemInstance, StackCount);
	for (TObjectPtr<ULyraInventoryItemInstance>& Result : Results)
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
}

void ULyraInventoryManagerComponent::RemoveItemInstance(ULyraInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<ULyraInventoryItemInstance*> ULyraInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

ULyraInventoryItemInstance* ULyraInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef) const
{
	for (const FLyraInventoryEntry& Entry : InventoryList.Entries)
	{
		ULyraInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 ULyraInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FLyraInventoryEntry& Entry : InventoryList.Entries)
	{
		ULyraInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool ULyraInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (ULyraInventoryItemInstance* Instance = ULyraInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void ULyraInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FLyraInventoryEntry& Entry : InventoryList.Entries)
		{
			ULyraInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool ULyraInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FLyraInventoryEntry& Entry : InventoryList.Entries)
	{
		ULyraInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


//////////////////////////////////////////////////////////////////////
//

// UCLASS(Abstract)
// class ULyraInventoryFilter : public UObject
// {
// public:
// 	virtual bool PassesFilter(ULyraInventoryItemInstance* Instance) const { return true; }
// };

// UCLASS()
// class ULyraInventoryFilter_HasTag : public ULyraInventoryFilter
// {
// public:
// 	virtual bool PassesFilter(ULyraInventoryItemInstance* Instance) const { return true; }
// };


