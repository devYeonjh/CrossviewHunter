// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentTileWidget.h"
#include "CHEquipmentOptionDisplayWidget.h"

#include "Equipment/LyraQuickBarComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "Equipment/LyraQuickBarComponent.h"
#include "Inventory/InventoryFragment_QuickBarIcon.h"
#include "Components/Image.h"
#include "Item/InventoryFragment_CHEquipmentInfo.h"


void UCHEquipmentTileWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCHEquipmentTileWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetQuickBarComponent();
	OptionWidgetInstance = CreateWidget<UCHEquipmentOptionDisplayWidget>(GetWorld(), OptionWidgetClass);
	SetToolTip(OptionWidgetInstance);
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	
	FGameplayTag ChannelTag = FGameplayTag::RequestGameplayTag(FName("Lyra.Inventory.Message.StackChanged"));
	MessageSubsystem.RegisterListener(ChannelTag, this, &UCHEquipmentTileWidget::UpdateTileData);

}

void UCHEquipmentTileWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCHEquipmentTileWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCHEquipmentTileWidget::GetQuickBarComponent()
{
	const TObjectPtr<ULyraQuickBarComponent> QuickBarComp =
		GetOwningPlayerPawn()->GetController()->GetComponentByClass<ULyraQuickBarComponent>();
	if (QuickBarComp)
	{
		QuickBarComponent = QuickBarComp;
	}
}

void UCHEquipmentTileWidget::UpdateTileData(FGameplayTag InChannel, const FLyraInventoryChangeMessage& InMessage)
{
	if (QuickBarComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("QuickBarComponent is nullptr"));
		return;
	}

	TArray<ULyraInventoryItemInstance*> ItemArray = QuickBarComponent->GetSlots();
	for (int32 i = 0; i < ItemArray.Num(); i++)
	{
		if (i == SlotNum)
		{
			if (ItemArray[i] == nullptr)
			{
				FSlateBrush SlateBrush;
				
				SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
				SlateBrush.Tiling = ESlateBrushTileType::NoTile;
				
				IconImage->SetBrush(SlateBrush);
			}
			else
			{
				const UInventoryFragment_QuickBarIcon* QuickBarIcon = ItemArray[i]->FindFragmentByClass<UInventoryFragment_QuickBarIcon>();
				if (QuickBarIcon == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("QuickBarIcon is nullptr"));
					break;
				}
				const FSlateBrush Brush = QuickBarIcon->Brush;
				IconImage->SetBrush(Brush);

				const UInventoryFragment_CHEquipmentInfo* EquipmentInfo = ItemArray[i]->FindFragmentByClass<UInventoryFragment_CHEquipmentInfo>();
				if (EquipmentInfo == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("EquipmentInfo is nullptr"));
					break;
				}
				OptionWidgetInstance->BindToEquipmentDefinition(EquipmentInfo->GetEquipmentDef());
			}
			
			break;
		}
	}
}
