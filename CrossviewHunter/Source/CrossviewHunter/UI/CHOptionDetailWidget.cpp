// Fill out your copyright notice in the Description page of Project Settings.


#include "CHOptionDetailWidget.h"

#include "GameplayEffect.h"
#include "Components/TextBlock.h"
#include "Item/CHItemDataTableRows.h"

void UCHOptionDetailWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCHOptionDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCHOptionDetailWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCHOptionDetailWidget::SetOptionText(const TPair<TSubclassOf<UGameplayEffect>, int32>& Option,
                                          const FCHItemOptionDetailRow& OptionDetail)
{
	FString OptionName = TrimGameplayEffectName(Option.Key->GetName());
	FString OptionText = FString::Printf(TEXT("%s +%d(%d ~ %d)"), *OptionName, Option.Value, OptionDetail.MinValue, OptionDetail.MaxValue);
	AdditionalOption->SetText(FText::FromString(OptionText));
}

FString UCHOptionDetailWidget::TrimGameplayEffectName(const FString& Source)
{
	FString Result = Source;
 
	// 앞에 "GE_"가 있으면 제거
	const FString Prefix = TEXT("GE_");
	if (Result.StartsWith(Prefix))
	{
		Result = Result.Mid(Prefix.Len());
	}
 
	// 뒤에 "_C"가 있으면 제거
	const FString Suffix = TEXT("_C");
	if (Result.EndsWith(Suffix))
	{
		Result = Result.Left(Result.Len() - Suffix.Len());
	}
 
	return Result;
}
