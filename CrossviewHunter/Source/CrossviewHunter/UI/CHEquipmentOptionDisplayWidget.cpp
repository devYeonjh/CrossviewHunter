// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentOptionDisplayWidget.h"

#include "CHOptionDetailWidget.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "Equipment/CHEquipmentDefinition.h"
#include "Item/CHItemDataTableRows.h"
#include "Item/InventoryFragment_CHEquipmentInfo.h"
#include "Utility/EnumHelpers.h"


void UCHEquipmentOptionDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCHEquipmentOptionDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UCHEquipmentOptionDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCHEquipmentOptionDisplayWidget::BindToEquipmentInfo(const UInventoryFragment_CHEquipmentInfo* EquipmentInfo)
{
	BoundEquipmentInfo = EquipmentInfo;
	if (BoundEquipmentInfo)
	{
		UpdateOptionData();
	}
}

const UInventoryFragment_CHEquipmentInfo* UCHEquipmentOptionDisplayWidget::GetEquipmentInfo() const
{
	return BoundEquipmentInfo;
}

void UCHEquipmentOptionDisplayWidget::UpdateOptionData()
{
	OptionBox->ClearChildren();

	if (OptionDetailWidget == nullptr)
	{
		return;
	}
	
	if (BoundEquipmentInfo)
	{
		// ItemData 정보 받아오기
		const FCHItemDataTableRow& ItemData = BoundEquipmentInfo->GetItemData();

		// 아이템 정보 텍스트 업데이트
		ItemName->SetText(ItemData.ItemName);
		ItemGrade->SetText(UEnumHelpers::GetEnumDisplayName(ItemData.ItemGrade));
		ItemType->SetText(UEnumHelpers::GetEnumDisplayName(ItemData.ItemType));

		// @TODO: 머티리얼로 수정
		switch (ItemData.ItemGrade)
		{
		case ECHGradeID::grade_20001:
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(0.62f, 0.62f, 0.62f)));
			break;
		case ECHGradeID::grade_20002:
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(0.17f, 0.58f, 0.81f)));
			break;
		case ECHGradeID::grade_20003:
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(0.54f, 0.f, 1.f)));
			break;
		case ECHGradeID::grade_20004:
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.8f)));
			break;
		case ECHGradeID::grade_20005:
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.84f, 0.f)));
			break;
		default: 
			ItemGrade->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f)));
			break;
		}
		
		TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions =
			BoundEquipmentInfo->GetEquipmentDef()->GetAdditionalOptions();
		TArray<FCHItemOptionDetailRow> OptionDetailList = BoundEquipmentInfo->GetEquipmentDef()->GetOptionDetails();
		
		for (int32 i = 0; i < AdditionalOptions.Num(); i++)
		{
			UCHOptionDetailWidget* NewObject = CreateWidget<UCHOptionDetailWidget>(this, OptionDetailWidget);
			OptionBox->AddChildToVerticalBox(NewObject);
			NewObject->SetOptionText(AdditionalOptions[i], OptionDetailList[i]);
		}
	}
}
