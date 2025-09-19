// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"

#include "CHStatusDisplayWidget.generated.h"

class UTextBlock;
class UCHStatSet;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHStatusDisplayWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> AttackText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> DefenceText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CritRateText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> FireRateText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ReloadRateText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ArmorPiercingDamageText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CritDamageText;

	UPROPERTY(BlueprintReadOnly, Category ="TEXT", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> HeadDamageText;

	UPROPERTY()
	TObjectPtr<const UCHStatSet> BoundStatSet;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void BindToStatSet(const UCHStatSet* StatSet);
	void UnbindFromStatSet();

private:
	void OnHealthChanged(float NewValue);
	void OnAttackChanged(float NewValue);
	void OnDefenceChanged(float NewValue);
	                                                            

	void UpdateAllStats();
};
