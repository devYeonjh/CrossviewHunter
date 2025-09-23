// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHItemDataTableRows.h"
#include "Engine/DataAsset.h"

#include "CHOptionPool.generated.h"



/**
* OptionPool 저장하는 데이터 에셋 (ItemOptionDetail DataTable에서 자동으로 데이터 가져옴)
 * @TODO 서버에서 DataTable 참조해서 미리 로딩하는 방식으로 변경
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "CH OptionPool"))
class UCHOptionPool : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCHOptionPool(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	ECHOptionPoolID PoolID;

	/** 옵션 Detail Array PostEditChangeProperty에 의해 자동으로 생성 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OptionPools" )
	TArray<FCHItemOptionDetailRow> OptionDetails;

	/** 옵션 Detail을 저장하는 데이터 테이블 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataSource")
	TObjectPtr<UDataTable> OptionDataTable;

	/** 등급 정보에 대한 데이터 테이블 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataSource")
	TObjectPtr<UDataTable> GradeDataTable;

public:
#if WITH_EDITOR
	// 에디터에서 PoolID 변경 시 호출
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	// 수동 새로고침
	UFUNCTION(CallInEditor, Category = "DataManagement")
	void RefreshOptionDetails();
#endif

	/** 등급에 따라 다른 갯수의 랜덤 옵션 결정 */
	TArray<TPair<ECHStatID, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> GetRandomOptions(const ECHGradeID Grade);


private:
	// 내부 로드 함수
	void LoadMatchingOptionsFromDataTable();
	
	// 이전 PoolID 추적 (변경 감지용)
	UPROPERTY()
	ECHOptionPoolID LastPoolID;
};

