// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "EditorSubsystem.h"

#include "CHPhaseDataEditorUtilityWidget.generated.h"

struct FCHMonsterSpawnPointRow;
class UCanvasPanelSlot;
class USizeBox;
class UImage;
class UMaterialInterface;
class ASceneCapture2D;
class UDataTable;
class UUserWidget;
class UCanvasPanel;
/**
 * 
 */
UCLASS(Blueprintable)
class CROSSVIEWHUNTEREDITOR_API UCHPhaseDataEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	// Spawn Point 정보가 있는 DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (RequiredAssetDataTags="RowStructure=/Script/CrossviewHunter.CHMonsterSpawnPointRow"))
	TObjectPtr<UDataTable> PhaseDataTable;

	// 현재 Phase에 해당하는 Spawn Point 정보들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	TArray<FCHMonsterSpawnPointRow> SpawnPointRowsData;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 맵의 캡처 이미지를 표시할 위젯
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> RenderImage;

	// 맵의 종횡비 고정을 위한 SizeBox
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> MapSizeBox;

	// Spawn Point를 화면에 배치할 CanvasPanel
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> SpawnPointPanel;

	// Capture Component의 MI를 적용할 Material
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BaseMaterial;

	// Spawn Point를 화면에 표시할 위젯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> SpawnPointWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, TObjectPtr<UCanvasPanelSlot>> SpawnPointWidgetMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentPhase = 1;

	// 현재 Phase 설정 및 Spawn Point Widget 업데이트
	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	FORCEINLINE void SetCurrentPhase(int32 Phase)
	{
		CurrentPhase = Phase;
		UpdateSpawnPointWidget();
	}
	
	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	int32 GetMaxPhaseNumber();

	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	TArray<FCHMonsterSpawnPointRow> GetSpawnPointRowsForCurrentPhase();

	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	UCanvasPanelSlot* GetSpawnPointWidget(const FName ID)
	{
		if (TObjectPtr<UCanvasPanelSlot>* Result = SpawnPointWidgetMap.Find(ID))
		{
			return *Result;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	void SaveSpawnPointRowsData();

	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	FVector2D ConvertWorldPosToCanvasPos(const FVector& WorldPosition);

	UFUNCTION(BlueprintCallable, Category = "CHPhaseDataEditor")
	FVector ConvertCanvasPosToWorldPos(const FVector2D& CanvasPosition);

	
private:
	UPROPERTY()
	TObjectPtr<ASceneCapture2D> SceneCaptureActor = nullptr;
	

	FVector2D BaseSize = FVector2D(20.0f, 20.0f);
	FVector MapCenter;
	FVector MapExtent;
	FVector CaptureCenter;			// SceneCapture2D의 중심 위치
	FVector2D CurrentCanvasSize;	// 현재 Canvas의 크기
	float CurrentCanvasScale;		// 현재 Canvas의 크기 비율
	float MapAspectRatio = 1.0f;	// 맵의 종횡비
	

	// 캡처 컴포넌트 초기화
	void InitializeCaptureComponent(UWorld* World, const TObjectPtr<ASceneCapture2D>& CaptureActor);

	// Spawn Point Widget 업데이트
	void UpdateSpawnPointWidget();

	void ClearSpawnPointWidget();

	FBox CalculateBounds(UWorld* World);

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnPointRowPropertyChangedDelegate, int32, ChangedIndex, FName, PropertyName);
 

UCLASS()
class UCHRowPropertySubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
 
public:
	UPROPERTY(BlueprintAssignable, Category="Spawn")
	FOnSpawnPointRowPropertyChangedDelegate OnSpawnPointRowPropertyChanged;
 
	// C++에서 델리게이트 브로드캐스트용 함수
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void BroadcastSpawnPointRowChanged(int32 Index, FName PropertyName)
	{
		OnSpawnPointRowPropertyChanged.Broadcast(Index, PropertyName);
	}
};
