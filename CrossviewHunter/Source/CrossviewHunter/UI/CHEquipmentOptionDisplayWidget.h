// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Item/CHItemTypes.h"
#include "CHEquipmentOptionDisplayWidget.generated.h"

class UCHEquipmentDefinition;
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
	TObjectPtr<UTextBlock> AdditionalOption1;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> AdditionalOption2;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> AdditionalOption3;

	UPROPERTY()
	TObjectPtr<const UCHEquipmentDefinition> BoundEquipmentDefinition;

	TArray<TObjectPtr<UTextBlock>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> TextBlocks;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "CH|OptionDisplayWidget")
	void BindToEquipmentDefinition(const UCHEquipmentDefinition* EquipmentDefinition);
	

private:
	
	void UpdateOptionData();
};
