// Fill out your copyright notice in the Description page of Project Settings.


#include "CHPlayerState.h"

#include "AbilitySystem/CHAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CHGoldSet.h"
#include "AbilitySystem/Attributes/CHLevelSet.h"
#include "AbilitySystem/Attributes/CHStatSet.h"
#include "Inventory/CHInventoryManagerComponent.h"
#include "System/CHGameInstance.h"

ACHPlayerState::ACHPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	StatSet = CreateDefaultSubobject<UCHStatSet>(TEXT("StatSet"));
	GoldSet = CreateDefaultSubobject<UCHGoldSet>(TEXT("GoldSet"));
	LevelSet = CreateDefaultSubobject<UCHLevelSet>(TEXT("LevelSet"));
}

void ACHPlayerState::OnDeactivated()
{
	APlayerController* PC = GetPlayerController();

	if (PC->HasLocalNetOwner())
	{
		if (UCHInventoryManagerComponent* InventoryManagerComponent = GetPlayerController()->GetComponentByClass<UCHInventoryManagerComponent>())
		{
			if (UCHGameInstance* GI = GetGameInstance<UCHGameInstance>())
			{
				GI->SavedPlayerData.InventoryList.Empty();
				GI->SavedPlayerData.InventoryList.Append(InventoryManagerComponent->GetAddableInventoryList());
			}
		}
	}
	
	Super::OnDeactivated();
}

void ACHPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	if (AController* PC = GetPlayerController())
	{
		if (UCHGameInstance* GI = GetGameInstance<UCHGameInstance>())
		{
			GI->InitAttributeSet(AbilitySystemComponent, TEXT("Default"), 2, true);
			
			if (UCHInventoryManagerComponent* InventoryManagerComponent = PC->GetComponentByClass<UCHInventoryManagerComponent>())
			{
				TArray<FLyraInventoryEntry>& SavedInventoryList = GI->SavedPlayerData.InventoryList;
				if (!SavedInventoryList.IsEmpty())
				{
					InventoryManagerComponent->GetAddableInventoryList().Append(SavedInventoryList);
					SavedInventoryList.Empty();
				}
			}
		}
	}
	
	Super::OnExperienceLoaded(CurrentExperience);
}

void ACHPlayerState::InitializeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCHAbilitySystemComponent>(this, TEXT("CHAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

