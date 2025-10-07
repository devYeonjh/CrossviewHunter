// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemCreationComponent.h"
#include "CHItemDefinition.h"
#include "CHItemInstance.h"
#include "CHPickableItem.h"
#include "CHOptionPool.h"
#include "CHItemDataTableRows.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "System/CHGameInstance.h"
#include "DataTable/CHDataTableManager.h"
#include "GameplayTagContainer.h"


UCHItemCreationComponent::UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UCHItemCreationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_LowPriority(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void UCHItemCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		if (UGameInstance* GI = GetGameInstance<UCHGameInstance>())
		{
			if (UCHDataTableManager* DataTableManager = GI->GetSubsystem<UCHDataTableManager>())
			{
				TArray<UDataTable*> ItemDataTables = DataTableManager->GetDataTableList(FGameplayTag::RequestGameplayTag("CH.DT.ItemDataTable"));
				if (ItemDataTables.Num() > 0)
				{
					CachedItemDataTableList = ItemDataTables;
				}
				TArray<UDataTable*> EquipmentTypeDataTables = DataTableManager->GetDataTableList(FGameplayTag::RequestGameplayTag("CH.DT.EquipmentTypeDataTable"));
				if (EquipmentTypeDataTables.Num() > 0)
				{
					CachedEquipmentTypeDataTableList = EquipmentTypeDataTables;
				
				}
			}
		}
	}
#endif
}

ACHPickableItem* UCHItemCreationComponent::SpawnPickableItem(const FName& ItemID, const FVector& Location, const FRotator& Rotation)
{
	ULyraInventoryItemInstance* ItemInst =  CreateItemInstance(ItemID);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TSubclassOf<ACHPickableItem> PickableClass = ACHPickableItem::StaticClass();
	
	ACHPickableItem* PickableItem = GetWorld()->SpawnActor<ACHPickableItem>(PickableClass, Location, Rotation, SpawnInfo);

	PickableItem->SetInventoryItemInfo(ItemInst);
	
	return PickableItem;
}

ACHPickableItem* UCHItemCreationComponent::SpawnPickableItemByClass
(const TSubclassOf<ACHPickableItem>& PickableClass, const FName& ItemID, const FVector& Location, const FRotator& Rotation)
{
	ULyraInventoryItemInstance* ItemInst =  CreateItemInstance(ItemID);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//TSubclassOf<ACHPickableItem> PickableClass = ACHPickableItem::StaticClass;
	
	ACHPickableItem* PickableItem = GetWorld()->SpawnActor<ACHPickableItem>(PickableClass, Location, Rotation, SpawnInfo);

	PickableItem->SetInventoryItemInfo(ItemInst);
	
	return PickableItem;
}


ULyraInventoryItemInstance* UCHItemCreationComponent::CreateItemInstance(const FName& ItemID)
{
	// 아이템 데이터 세팅
	UCHItemDefinition* ItemDefinition = NewObject<UCHItemDefinition>();
	const FCHItemDataTableRow& ItemDataRow = FindItemDataByID(ItemID);
	ItemDefinition->SetItemData(this, ItemDataRow);
	
	// 아이템 인스턴스 생성
	UCHItemInstance* ItemInstance = NewObject<UCHItemInstance>();
	ItemInstance->SetItemDefinitionData(ItemDefinition);
	
	CreatedItems.Add(ItemInstance);
	
	return ItemInstance;
}

TSubclassOf<UGameplayEffect> UCHItemCreationComponent::GetStatEffect(const ECHStatID StatID)
{
	if (StatEffectMap.Contains(StatID) == false)
		return nullptr;
	return StatEffectMap[StatID];
}

TObjectPtr<UCHOptionPool> UCHItemCreationComponent::GetOptionPool(const ECHOptionPoolID OptionPoolID)
{
	if (OptionPools.Contains(OptionPoolID) == false)
		return nullptr;
	return OptionPools[OptionPoolID];
}

const FCHItemDataTableRow& UCHItemCreationComponent::FindItemDataByID(const FName& ItemID) const
{
	const FCHItemDataTableRow* OutItemDataRow = nullptr;
	for (UDataTable* DataTable : CachedItemDataTableList)
	{
		OutItemDataRow = DataTable->FindRow<FCHItemDataTableRow>(ItemID, TEXT("FindItemDataByID"));
		if (OutItemDataRow)
		{
			return *OutItemDataRow;
		}
	}
	return *OutItemDataRow;
}

const FCHEquipmentTypeDefinitionRow& UCHItemCreationComponent::FindEquipmentTypeDefinition(ECHItemType Type) const
{
	const FCHEquipmentTypeDefinitionRow* OutEquipmentTypeDefinitionRow = nullptr;
	
	const UEnum* EnumClass = StaticEnum<ECHItemType>();
	check(EnumClass != nullptr);
	FName TypeName = FName(EnumClass->GetNameStringByValue(static_cast<int64>(Type)));

	for (UDataTable* DataTable : CachedEquipmentTypeDataTableList)
	{
		OutEquipmentTypeDefinitionRow = DataTable->FindRow<FCHEquipmentTypeDefinitionRow>(TypeName, TEXT("FindEquipmentTypeDefinition"));
		if (OutEquipmentTypeDefinitionRow)
		{
			return *OutEquipmentTypeDefinitionRow;
		}
	}

	return *OutEquipmentTypeDefinitionRow;
}
