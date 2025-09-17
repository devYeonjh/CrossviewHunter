// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CHItemTypes.h"
#include "Components/GameStateComponent.h"
#include "AbilitySystem/CHStatID.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CHItemCreationComponent.generated.h"

class UCHStatEffectBase;
class UCHItemDefinition;
class ULyraInventoryItemFragment;
class ULyraExperienceDefinition;
class ULyraInventoryItemDefinition;
class ULyraInventoryItemInstance;
class ACHPickableItem;
struct FItemDataTableRow;
struct FEquipmentTypeDefinitionRow;

/**
 * 아이템 생성기 컴포넌트 (GameStateComponent)
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class CROSSVIEWHUNTER_API UCHItemCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UCHItemCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

protected:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);

public:
	/** 아이템 액터(CHPickableItem) 스폰 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	ACHPickableItem* SpawnPickableItem(const FName& ItemID, const FVector& Location, const FRotator& Rotation);

	/** 아이템 인스턴스 생성 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	ULyraInventoryItemInstance* CreateItemInstance(const FName& ItemID);
	
private:
	/** ID 기반으로 Data 찾기*/
	FItemDataTableRow& FindItemDataByID(const FName& ItemID) const;

	/** Type 정보 받아오기 */
	FEquipmentTypeDefinitionRow& FindEquipmentTypeDefinition(ECHItemType Type) const;

	
protected:
	/** DataTable containing item definitions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TObjectPtr<UDataTable> ItemDataTable;

	/** DataTable containing item definitions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TObjectPtr<UDataTable> EquipmentTypeDataTable;

	void SetEquipmentFragment(UCHItemDefinition& ItemDef, const FItemDataTableRow& DataTableRow);

	
	
	/** Array to track created item instances for testing */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ULyraInventoryItemInstance>> CreatedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TMap<ECHStatID, TSubclassOf<UCHStatEffectBase>> StatEffectMap;

};