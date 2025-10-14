// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Item/CHItemTypes.h"
#include "CHOptionDetailWidget.generated.h"

struct FCHItemOptionDetailRow;
class UGameplayEffect;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHOptionDetailWidget : public UCommonUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> AdditionalOption;

public:
	// ~UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~End of UUserWidget Interface
	
	void SetOptionText(const TPair<TSubclassOf<UGameplayEffect>, int32>& Option, const FCHItemOptionDetailRow& OptionDetail);

private:
	FString TrimGameplayEffectName(const FString& Source);
};
