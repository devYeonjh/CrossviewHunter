// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CHOptionChangeScreen.generated.h"

class UCHEquipmentOptionDisplayWidget;
class UButton;
/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHOptionChangeScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ChangeBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCHEquipmentOptionDisplayWidget> EquipmentDataWidget;
	
public:
	// ~UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~End of UUserWidget Interface

protected:
	UFUNCTION()
	void ChangeRandomOption();
	
};
