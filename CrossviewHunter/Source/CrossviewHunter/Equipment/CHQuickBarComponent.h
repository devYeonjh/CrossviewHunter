// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LyraQuickBarComponent.h"
#include "CHQuickBarComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROSSVIEWHUNTER_API UCHQuickBarComponent : public ULyraQuickBarComponent
{
	GENERATED_BODY()

protected:
	virtual void EquipItemInSlot() override;
	virtual void UnequipItemInSlot() override;
};
