// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CHItemTypes.h"
#include "Components/GameStateComponent.h"
#include "AbilitySystem/CHStatID.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Attributes/CHStatEffectBase.h"
#include "CHOptionPool.h"

#include "CHItemCreationComponent.generated.h"

struct FCHEquipmentTypeDefinitionRow;
struct FCHItemDataTableRow;
class UCHItemDefinition;
class ULyraInventoryItemFragment;
class ULyraExperienceDefinition;
class ULyraInventoryItemDefinition;
class ULyraInventoryItemInstance;
class ACHPickableItem;


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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	ACHPickableItem* SpawnPickableItemByClass(const TSubclassOf<ACHPickableItem>& PickableClass, const FName& ItemID, const FVector& Location, const FRotator& Rotation);

	/** 아이템 인스턴스 생성 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = ItemCreation)
	ULyraInventoryItemInstance* CreateItemInstance(const FName& ItemID);

	/** GameEffect 받아오기 */
	TSubclassOf<UGameplayEffect> GetStatEffect(const ECHStatID StatID);

	TObjectPtr<UCHOptionPool> GetOptionPool(const ECHOptionPoolID OptionPoolID);

	/** Type 정보 받아오기 */
	FCHEquipmentTypeDefinitionRow& FindEquipmentTypeDefinition(ECHItemType Type) const;
	
private:
	/** ID 기반으로 Data 찾기*/
	FCHItemDataTableRow& FindItemDataByID(const FName& ItemID) const;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TObjectPtr<UDataTable> EquipmentTypeDataTable;
	
	/** Array to track created item instances for testing */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ULyraInventoryItemInstance>> CreatedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TMap<ECHStatID, TSubclassOf<UCHStatEffectBase>> StatEffectMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemCreation)
	TMap<ECHOptionPoolID, TObjectPtr<UCHOptionPool>> OptionPools;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSubclassOf<ACHPickableItem> PickableClass;
};