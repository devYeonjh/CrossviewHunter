// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CHStatID.h"
#include "CHItemTypes.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "Engine/DataAsset.h"

#include "CHItemDataTableRows.generated.h"

class ULyraAbilitySet;
class ULyraEquipmentInstance;
class ULyraInventoryItemDefinition;
struct FFrame;

UENUM(BlueprintType)
enum class ECHGradeID : uint8
{
	grade_20001 UMETA(DisplayName = "Common"),
	
	grade_20002 UMETA(DisplayName = "Rare"),
	
	grade_20003 UMETA(DisplayName = "Epic"),
	
	grade_20004 UMETA(DisplayName = "Unique"),
	
	grade_20005 UMETA(DisplayName = "Legendary"),
	
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECHOptionPoolID : uint8
{
	pool_200001 UMETA(DisplayName = "Weapon_Common_Options"),

	pool_200002 UMETA(DisplayName = "Armor_Common_Options"),

	pool_200003 UMETA(DisplayName = "Weapon_Unique_Options"),

	pool_200004 UMETA(DisplayName = "Armor_Unique_Options"),

	MAX UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FCHItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHItemType ItemType = ECHItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grade")
	ECHGradeID ItemGrade = ECHGradeID::grade_20001;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHEquipmentSlot EquipmentSlot = ECHEquipmentSlot::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TMap<ECHStatID, float> BaseStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHOptionPoolID OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FName SpecialEffectID;
};

USTRUCT(BlueprintType)
struct FCHItemOptionDetailRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FName OptionID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHOptionPoolID OptionPoolID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	ECHStatID StatID = ECHStatID::stat_001;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	int32 Weight;
	
};

USTRUCT(BlueprintType)
struct FCHEquipmentTypeDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	ECHItemType Type = ECHItemType::None;
	
	// Class to spawn
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSoftClassPtr<ULyraEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<TSoftObjectPtr<ULyraAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSoftClassPtr<ULyraInventoryItemDefinition> BaseItemDefinition;
};

USTRUCT(BlueprintType)
struct FCHItemGradeRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHGradeID ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	int32 AffixLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	int32 ScrapMetal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FString Description;
};


/**
 * OptionPool 저장하는 데이터 에셋 (ItemOptionDetail DataTable에서 자동으로 데이터 가져옴)
 * @TODO 서버에서 DataTable 참조해서 미리 로딩하는 방식으로 변경
 */
UCLASS(Blueprintable)
class UCHOptionPool : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCHOptionPool(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHOptionPoolID PoolID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OptionPools" )
	TArray<FCHItemOptionDetailRow> OptionDetails;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataSource")
	TObjectPtr<UDataTable> OptionDataTable;

public:
#if WITH_EDITOR
	// 에디터에서 PoolID 변경 시 호출
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	// 수동 새로고침
	UFUNCTION(CallInEditor, Category = "DataManagement")
	void RefreshOptionDetails();
#endif

	TMap<ECHStatID, int32> GetRandomOptions(int32 Num);


private:
	// 내부 로드 함수
	void LoadMatchingOptionsFromDataTable();
	
	// 이전 PoolID 추적 (변경 감지용)
	UPROPERTY()
	ECHOptionPoolID LastPoolID;
};

