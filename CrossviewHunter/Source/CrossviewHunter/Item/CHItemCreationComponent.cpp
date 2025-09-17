// Fill out your copyright notice in the Description page of Project Settings.


#include "CHItemCreationComponent.h"
#include "CHItemDefinition.h"
#include "CHItemInstance.h"
#include "CHPickableItem.h"
#include "InventoryFragment_CHEquipmentInfo.h"
#include "Equipment/CHEquipmentManagerComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "UObject/ConstructorHelpers.h"



UCHItemCreationComponent::UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObj(TEXT("/Game/CrossviewHunter/Core/DT_ItemData.DT_ItemData"));
	if (ItemDataTableObj.Succeeded())
	{
		ItemDataTable = ItemDataTableObj.Object;
	}
	ConstructorHelpers::FObjectFinder<UDataTable> EquipmentTypeDataTableObj(TEXT("/Game/CrossviewHunter/Core/DT_EquipmentType.DT_EquipmentType"));
	if (EquipmentTypeDataTableObj.Succeeded())
	{
		EquipmentTypeDataTable = EquipmentTypeDataTableObj.Object;
	}
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
	//@TODO: 서버 코드 : 아이템 데이터 테이블 로드
}

ACHPickableItem* UCHItemCreationComponent::SpawnPickableItem(const FName& ItemID, const FVector& Location, const FRotator& Rotation)
{
	ULyraInventoryItemInstance* ItemInst =  CreateItemInstance(ItemID);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TSubclassOf<ACHPickableItem> PickableClass = ACHPickableItem::StaticClass();
	ACHPickableItem* PickableItem = GetWorld()->SpawnActor<ACHPickableItem>(PickableClass, Location, Rotation, SpawnInfo);

	PickableItem->SetStaticMesh(TestMesh.Get());
	PickableItem->SetInventoryItemInfo(ItemInst);

	return PickableItem;
}

ULyraInventoryItemInstance* UCHItemCreationComponent::CreateItemInstance(const FName& ItemID)
{
	// 아이템 데이터 세팅
	UCHItemDefinition* ItemDefinition = NewObject<UCHItemDefinition>();
	const FItemDataTableRow& ItemDataRow = FindItemDataByID(ItemID);
	ItemDefinition->DisplayName = ItemDataRow.ItemName;
	SetEquipmentFragment(*ItemDefinition, ItemDataRow);
	
	
	// 아이템 인스턴스 생성
	UCHItemInstance* ItemInstance = NewObject<UCHItemInstance>();
	ItemInstance->SetItemDefinitionData(ItemDefinition);
	
	CreatedItems.Add(ItemInstance);
	
	return ItemInstance;
}

FItemDataTableRow& UCHItemCreationComponent::FindItemDataByID(const FName& ItemID) const
{
	FItemDataTableRow* OutItemDataRow = new FItemDataTableRow();
	UDataTableFunctionLibrary::Generic_GetDataTableRowFromName(ItemDataTable, ItemID, OutItemDataRow);

	return *OutItemDataRow;
}

FEquipmentTypeDefinitionRow& UCHItemCreationComponent::FindEquipmentTypeDefinition(ECHItemType Type) const
{
	FEquipmentTypeDefinitionRow* OutEquipmentTypeDefinitionRow = new FEquipmentTypeDefinitionRow();
	
	const UEnum* EnumClass = StaticEnum<ECHItemType>();
	check(EnumClass != nullptr);
	FName TypeName = FName(EnumClass->GetNameStringByValue(static_cast<int64>(Type)));
	UDataTableFunctionLibrary::Generic_GetDataTableRowFromName(EquipmentTypeDataTable, TypeName, OutEquipmentTypeDefinitionRow);

	return *OutEquipmentTypeDefinitionRow;
}

void UCHItemCreationComponent::SetEquipmentFragment(UCHItemDefinition& ItemDef, const FItemDataTableRow& DataTableRow)
{
	// EquipmentInfo Fragment
	TObjectPtr<UInventoryFragment_CHEquipmentInfo> EquipInfo = NewObject<UInventoryFragment_CHEquipmentInfo>();
	EquipInfo->InitializeValue(DataTableRow.ItemType, DataTableRow.EquipmentSlot, DataTableRow.BaseStats);
	FEquipmentTypeDefinitionRow EquipTypeDefinition = FindEquipmentTypeDefinition(DataTableRow.ItemType);
	EquipInfo->SetEquipmentDefinitionByData(EquipTypeDefinition);
	

	ItemDef.Fragments.Add(EquipInfo);
	if (UClass* DefinitionClass = EquipTypeDefinition.BaseItemDefinition.LoadSynchronous())
	{
		if (ULyraInventoryItemDefinition* DefaultObject = DefinitionClass->GetDefaultObject<ULyraInventoryItemDefinition>())
		{
			ItemDef.Fragments.Append(DefaultObject->Fragments);
		}
	}

}
