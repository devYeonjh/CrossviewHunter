// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CHStatID.h"
#include "AttributeSet.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "CHAttributeHelpers.generated.h"




/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UCHAttributeHelpers : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	FGameplayAttribute GetAttributeForID(ECHStatID ID);

private:
	void InitializeDataTable();
	
	TSoftObjectPtr<UDataTable> StatDataTable;
	TMap<ECHStatID, FGameplayAttribute> StatAttributeMap;
	
};
