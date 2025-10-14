// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CHEquipmentOptionDisplayWidget.generated.h"

class UVerticalBox;
class UInventoryFragment_CHEquipmentInfo;
class ULyraInventoryItemInstance;
class UCHOptionDetailWidget;
class UTextBlock;
class UCHStatSet;


/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHEquipmentOptionDisplayWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ItemType;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ItemGrade;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> OptionBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCHOptionDetailWidget> OptionDetailWidget;
	
	UPROPERTY()
	TObjectPtr<const UInventoryFragment_CHEquipmentInfo> BoundEquipmentInfo;


public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "CH|OptionDisplayWidget")
	void BindToEquipmentInfo(const UInventoryFragment_CHEquipmentInfo* EquipmentInfo);

	UFUNCTION(BlueprintCallable, Category = "CH|OptionDisplayWidget")
	const UInventoryFragment_CHEquipmentInfo* GetEquipmentInfo() const;

	UFUNCTION(BlueprintCallable, Category = "CH|OptionDisplayWidget")
	void UpdateOptionData();
	
};
