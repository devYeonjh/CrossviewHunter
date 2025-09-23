// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CHEquipmentTileWidget.generated.h"

class UImage;
struct FLyraInventoryChangeMessage;
struct FGameplayTag;
class UCHEquipmentOptionDisplayWidget;
class ULyraQuickBarComponent;
class UCHEquipmentDefinition;
/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHEquipmentTileWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	int32 SlotNum = 0;

	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	void GetQuickBarComponent();

	UFUNCTION(BlueprintCallable, Category = "CH|EquipmentTileWidget")
	void UpdateTileData(FGameplayTag InChannel, const FLyraInventoryChangeMessage& InMessage);
	
protected:
	UPROPERTY()
	TObjectPtr<UCHEquipmentDefinition> BoundEquipmentDefinition;
	UPROPERTY()
	TObjectPtr<ULyraQuickBarComponent> QuickBarComponent;
	UPROPERTY(EditDefaultsOnly, Category = "OptionDisplayWidget")
	TSubclassOf<UCHEquipmentOptionDisplayWidget> OptionWidgetClass;
	UPROPERTY()
	TObjectPtr<UCHEquipmentOptionDisplayWidget> OptionWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot" , meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> IconImage = nullptr;
};
