// Fill out your copyright notice in the Description page of Project Settings.


#include "CHEquipmentOptionDisplayWidget.h"

#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Equipment/CHEquipmentDefinition.h"


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

void UCHEquipmentOptionDisplayWidget::BindToEquipmentDefinition(const UCHEquipmentDefinition* EquipmentDefinition)
{
	BoundEquipmentDefinition = EquipmentDefinition;
	if (BoundEquipmentDefinition)
	{
		UpdateOptionData();
	}
}

void UCHEquipmentOptionDisplayWidget::UpdateOptionData()
{
	TextBlocks.Empty();
	TextBlocks.Emplace(AdditionalOption1);
	TextBlocks.Emplace(AdditionalOption2);
	TextBlocks.Emplace(AdditionalOption3);
	
	if (BoundEquipmentDefinition)
	{
		TArray<TPair<TSubclassOf<UGameplayEffect>, float>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions =
			BoundEquipmentDefinition->GetAdditionalOptions();

		int32 i = 0;
		for ( ; i < AdditionalOptions.Num(); i++)
		{
			FString OptionText = FString::Printf(TEXT("%s: %f"), *AdditionalOptions[i].Key->GetName(), AdditionalOptions[i].Value);
			TextBlocks[i]->SetText(FText::FromString(OptionText));
		}
		for ( ; i < MAX_ADDITIONAL_OPTION_COUNT; i++)
		{
			TextBlocks[i]->SetText(FText());
		}
	}
}
