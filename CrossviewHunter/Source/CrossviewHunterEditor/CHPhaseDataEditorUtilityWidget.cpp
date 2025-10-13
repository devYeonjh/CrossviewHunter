// Fill out your copyright notice in the Description page of Project Settings.


#include "CHPhaseDataEditorUtilityWidget.h"

#include "Landscape.h"
#include "Components/Image.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/LODActor.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Monster/CHMonsterSpawnPointRow.h"


void UCHPhaseDataEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World && !SceneCaptureActor)
	{
		FActorSpawnParameters Params;
		SceneCaptureActor = World->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass(), Params);

		InitializeCaptureComponent(World, SceneCaptureActor);
	}
}

void UCHPhaseDataEditorUtilityWidget::NativeDestruct()
{
	Super::NativeDestruct();


	if (SceneCaptureActor)
	{
		SceneCaptureActor->Destroy();
		SceneCaptureActor = nullptr;
	}
}

void UCHPhaseDataEditorUtilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D NewCanvasSize = SpawnPointPanel->GetCachedGeometry().GetLocalSize();
	if (!CurrentCanvasSize.Equals(NewCanvasSize, 0.01f))
	{
		float PrevCanvasScale = CurrentCanvasScale;
		CurrentCanvasSize = NewCanvasSize;
		CurrentCanvasScale = CurrentCanvasSize.Y * 0.001f; // Y축기준으로 1000.0f가 기본크기
		
		// SpawnPointWidgetMap의 CanvasSlot 위치 업데이트
		for (auto& Pair : SpawnPointWidgetMap)
		{
			if (UCanvasPanelSlot* CanvasSlot = Pair.Value)
			{
				CanvasSlot->SetSize(BaseSize * CurrentCanvasScale);
				FVector2D CurPos = CanvasSlot->GetPosition();
				CanvasSlot->SetPosition(CurPos * CurrentCanvasScale / PrevCanvasScale);
			}
		}
	}
}

int32 UCHPhaseDataEditorUtilityWidget::GetMaxPhaseNumber()
{
	int32 MaxPhase = 0;
	TArray<FCHMonsterSpawnPointRow*> SpawnPointRows;
	PhaseDataTable->GetAllRows<FCHMonsterSpawnPointRow>(TEXT("SpawnPoint"), SpawnPointRows);
	for (const FCHMonsterSpawnPointRow* Row : SpawnPointRows)
	{
		MaxPhase = FMath::Max(MaxPhase, Row->PhaseNumber);
	}
	return MaxPhase;	
}

TArray<FCHMonsterSpawnPointRow> UCHPhaseDataEditorUtilityWidget::GetSpawnPointRowsForCurrentPhase()
{
	TArray<FCHMonsterSpawnPointRow> ResultRows;
	if (PhaseDataTable)
	{
		TArray<FCHMonsterSpawnPointRow*> AllRows;
		PhaseDataTable->GetAllRows<FCHMonsterSpawnPointRow>(TEXT("SpawnPoint"), AllRows);
		for (FCHMonsterSpawnPointRow* Row : AllRows)
		{
			if (Row->PhaseNumber == CurrentPhase)
			{
				ResultRows.Add(*Row);
			}
		}
	}
	return ResultRows;
}

void UCHPhaseDataEditorUtilityWidget::SaveSpawnPointRowsData()
{
	if (PhaseDataTable)
	{
		PhaseDataTable->Modify();
		for (const FCHMonsterSpawnPointRow& Row : SpawnPointRowsData)
		{
			if (FCHMonsterSpawnPointRow* RowPtr = PhaseDataTable->FindRow<FCHMonsterSpawnPointRow>(Row.SpawnPointID, TEXT("")))
			{
				*RowPtr = Row; // 값 복사
			}
			else
			{
				PhaseDataTable->AddRow(Row.SpawnPointID, Row);
			}
		}
		PhaseDataTable->MarkPackageDirty(); // 저장 필요 표시
	}
}


void UCHPhaseDataEditorUtilityWidget::InitializeCaptureComponent(UWorld* World, const TObjectPtr<ASceneCapture2D>& CaptureActor)
{
	if (!World || !CaptureActor)
	{
		return;
	}

	// 맵의 Bounds 계산
	FBox Bounds = CalculateBounds(World);

	if (!Bounds.IsValid)
	{
		return;
	}
 
	MapCenter = Bounds.GetCenter();
	MapExtent = Bounds.GetExtent();
	
	CaptureCenter = FVector(MapCenter.X, MapCenter.Y, 0.0f);
	MapAspectRatio = MapExtent.Y / FMath::Max(MapExtent.X, 1.0f); // 맵의 종횡비 계산 (Width / Height)

	// MapSizeBox의 종횡비 설정 (이미지 크기 비율 고정)
	MapSizeBox->SetMinAspectRatio(MapAspectRatio);
	MapSizeBox->SetMaxAspectRatio(MapAspectRatio);

	// 위치 및 회전 명확히 세팅 (높이 위로 올리고 아래쪽으로 회전)
	float HeightOffset = 1000.f;
	FVector CameraLocation = FVector(MapCenter.X, MapCenter.Y, HeightOffset);
	FRotator CameraRotation = FRotator(-90.f, 0.f, 0.f);

	CaptureActor->SetActorLocation(CameraLocation);
	CaptureActor->SetActorRotation(CameraRotation);

	if (USceneCaptureComponent2D* CaptureComp = CaptureActor->GetCaptureComponent2D())
	{
		CaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
		CaptureComp->OrthoWidth = MapExtent.Y * 2.0f; // 가로 크기

		// Editor Utility Widget에서는 한 번만 캡처하면 되므로 자동 캡처 비활성화
		CaptureComp->bCaptureEveryFrame = false;
		CaptureComp->bCaptureOnMovement = false;

		// Shadow 비활성화
		TArray<FEngineShowFlagsSetting> FlagsToDisable = {
			FEngineShowFlagsSetting(TEXT("Shadows"), false),
			FEngineShowFlagsSetting(TEXT("DynamicShadows"), false),
			FEngineShowFlagsSetting(TEXT("StaticShadows"), false),
			FEngineShowFlagsSetting(TEXT("CapsuleShadows"), false),
			FEngineShowFlagsSetting(TEXT("ContactShadows"), false),
			FEngineShowFlagsSetting(TEXT("IndirectShadows"), false),
			FEngineShowFlagsSetting(TEXT("VolumetricShadows"), false),
			FEngineShowFlagsSetting(TEXT("FarShadows"), false),
			FEngineShowFlagsSetting(TEXT("VirtualShadowMaps"), false)
		};
		CaptureComp->SetShowFlagSettings(FlagsToDisable);

		// Skeletal Mesh 및 Instanced Static Mesh 비활성화
		CaptureComp->ShowFlags.SetSkeletalMeshes(false);
		CaptureComp->ShowFlags.SetInstancedStaticMeshes(false);

		// Render Target 생성 및 할당
		UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
		RenderTarget->ClearColor = FLinearColor::Black;
		RenderTarget->InitAutoFormat(FMath::RoundToInt(2048 * MapAspectRatio), 2048); //종횡비에 맞게 해상도 설정
		RenderTarget->UpdateResourceImmediate(true);

		CaptureComp->TextureTarget = RenderTarget;
		CaptureComp->CaptureScene();

		// Material 인스턴스 생성 및 브러시 설정
		if (BaseMaterial && RenderTarget)
		{
			UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			DynMaterial->SetTextureParameterValue(FName("RenderTargetParam"), RenderTarget);
			RenderImage->SetBrushFromMaterial(DynMaterial);
		}
	}
}

void UCHPhaseDataEditorUtilityWidget::UpdateSpawnPointWidget()
{
	// SpawnPointWidgetMap 초기화
	ClearSpawnPointWidget();
	
	if (!SpawnPointWidgetClass || !SpawnPointPanel || !PhaseDataTable)
	{
		return;
	}

	// PhaseDataTable에서 SpawnPointRow 가져오기
	TArray<FCHMonsterSpawnPointRow*> SpawnPointRows;
	PhaseDataTable->GetAllRows<FCHMonsterSpawnPointRow>(TEXT("SpawnPoint"), SpawnPointRows);

	// SpawnPointWidgetMap에 데이터 할당
	for (FCHMonsterSpawnPointRow* SpawnPointRow : SpawnPointRows)
	{
		// 현재 페이즈가 아닌 경우 스킵
		if (SpawnPointRow->PhaseNumber != CurrentPhase)
		{
			continue;
		}

		// Widget 생성 및 CanvasSlot 설정
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, SpawnPointWidgetClass);
		if (UCanvasPanelSlot* CanvasSlot = SpawnPointPanel->AddChildToCanvas(NewWidget))
		{
			// 현재 Canvas 크기 및 스케일 초기화 (Initialize때 기준으로 scale 설정)
			CurrentCanvasSize = SpawnPointPanel->GetCachedGeometry().GetLocalSize();
			CurrentCanvasScale = CurrentCanvasSize.Y * 0.001f; // Y축기준으로 1000.0f가 기본크기

			// WorldPosition을 Canvas 기준으로 비율 계산
			const FVector2D CanvasPosition = ConvertWorldPosToCanvasPos(SpawnPointRow->SpawnLocation);

			CanvasSlot->SetSize(BaseSize * CurrentCanvasScale);
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetPosition(CanvasPosition);

			SpawnPointWidgetMap.Add(SpawnPointRow->SpawnPointID, CanvasSlot);
		}
	}
}

void UCHPhaseDataEditorUtilityWidget::ClearSpawnPointWidget()
{
	for (auto& Pair : SpawnPointWidgetMap)
	{
		if (UCanvasPanelSlot* CanvasSlot = Pair.Value)
		{
			if (UUserWidget* Widget = Cast<UUserWidget>(CanvasSlot->GetContent()))
			{
				Widget->RemoveFromParent();
				Widget->Destruct();
			}
		}
	}
	SpawnPointWidgetMap.Empty();
}

FBox UCHPhaseDataEditorUtilityWidget::CalculateBounds(UWorld* World)
{
	FBox TotalBounds(EForceInit::ForceInit);

	if (!World)
    {
        return TotalBounds;
    }
    
    // 1. Landscape Bounds 우선 사용
    TArray<AActor*> Landscapes;
    UGameplayStatics::GetAllActorsOfClass(World, ALandscape::StaticClass(), Landscapes);
    if (Landscapes.Num() > 0)
    {
        for (AActor* Actor : Landscapes)
        {
            if (ALandscape* Landscape = Cast<ALandscape>(Actor))
            {
                TotalBounds += Landscape->GetCompleteBounds();
            }
        }
        return TotalBounds;
    }
 
    // 2. LODActor가 존재하면 SubActors의 StaticMeshActor 기준 Bounds 합산
    TArray<AActor*> HLODActors;
    UGameplayStatics::GetAllActorsOfClass(World, ALODActor::StaticClass(), HLODActors);
    if (HLODActors.Num() > 0)
    {
        for (AActor* Actor : HLODActors)
        {
            ALODActor* HLODActor = Cast<ALODActor>(Actor);
            if (HLODActor && HLODActor->SubActors.Num() > 0)
            {
                for (AActor* SubActor : HLODActor->SubActors)
                {
                    AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(SubActor);
                    if (MeshActor && MeshActor->GetStaticMeshComponent())
                    {
                        FBoxSphereBounds Bounds = MeshActor->GetStaticMeshComponent()->CalcBounds(MeshActor->GetActorTransform());
                        TotalBounds += Bounds.GetBox();
                    }
                }
            }
        }
        return TotalBounds;
    }
    
    // 3. Landscape, LODActor 모두 없을 때: 월드 내 모든 StaticMeshActor Bounds 합산 (필터링 포함)
    TArray<AActor*> MeshActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), MeshActors);
    for (AActor* Actor : MeshActors)
    {
        // 필요시 불필요한 Actor 제외 (예: Volume, SkyAtmosphere 등)
        if (Actor->IsA(AVolume::StaticClass()) || Actor->IsA(ASkyAtmosphere::StaticClass()))
        {
            continue;
        }
        
        AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);
        if (MeshActor && MeshActor->GetStaticMeshComponent())
        {
            FBoxSphereBounds Bounds = MeshActor->GetStaticMeshComponent()->CalcBounds(MeshActor->GetActorTransform());
            TotalBounds += Bounds.GetBox();
        }
    }
    
    return TotalBounds;
}


FVector2D UCHPhaseDataEditorUtilityWidget::ConvertWorldPosToCanvasPos(const FVector& WorldPosition)
{
	// WorldPosition을 Map의 중심 기준으로 비율 계산
	float HeightRatio = (WorldPosition.X - MapCenter.X) / MapExtent.X;
	float WidthRatio = (WorldPosition.Y - MapCenter.Y) / MapExtent.Y;

	// CanvasPos = Canvas Center + Canvas Extent * Ratio
	// 기준 Canvas(Y축 1000.0f)계산 이후 Scale 적용
	FVector2D CanvasPos = FVector2D(
		500.0f * MapAspectRatio * (1.0f + WidthRatio),
		500.0f * (1.0f - HeightRatio));
	
	return CanvasPos * CurrentCanvasScale;
}

FVector UCHPhaseDataEditorUtilityWidget::ConvertCanvasPosToWorldPos(const FVector2D& CanvasPosition)
{
	FVector2D BaseCanvasPos = CanvasPosition / CurrentCanvasScale;
	
	float HeightRatio = 1.0f - BaseCanvasPos.Y / 500.0f;
	float WidthRatio = BaseCanvasPos.X / (500.0f * MapAspectRatio) - 1.0f;

	FVector WorldPosition = FVector(
		MapCenter.Y + MapExtent.Y * HeightRatio,
		MapCenter.X + MapExtent.X * WidthRatio,
		0.0f
	);
	
	return WorldPosition;
}