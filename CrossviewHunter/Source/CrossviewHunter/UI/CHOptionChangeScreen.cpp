// Fill out your copyright notice in the Description page of Project Settings.


#include "CHOptionChangeScreen.h"

#include "CHEquipmentOptionDisplayWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/LyraGameState.h"
#include "Item/CHItemCreationComponent.h"
#include "Item/InventoryFragment_CHEquipmentInfo.h"
#include "Kismet/GameplayStatics.h"

void UCHOptionChangeScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCHOptionChangeScreen::NativeConstruct()
{
	Super::NativeConstruct();

	ChangeBtn->OnClicked.AddDynamic(this, &UCHOptionChangeScreen::ChangeRandomOption);
}

void UCHOptionChangeScreen::NativeDestruct()
{
	Super::NativeDestruct();

	ChangeBtn->OnClicked.Clear();
}

void UCHOptionChangeScreen::ChangeRandomOption()
{
	if (AGameStateBase* GS = UGameplayStatics::GetGameState(this))
	{
		if (UCHItemCreationComponent* ICC = GS->GetComponentByClass<UCHItemCreationComponent>())
		{
			const UInventoryFragment_CHEquipmentInfo* EquipInfo = EquipmentDataWidget->GetEquipmentInfo();
			const FCHItemDataTableRow ItemData = EquipInfo->GetItemData();
			// Additional 옵션 추가
			TObjectPtr<UCHOptionPool> OptionPool = ICC->GetOptionPool(ItemData.OptionPoolID);
			TArray<FCHItemOptionDetailRow> OptionDetails;
			TArray<TPair<ECHStatID, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> RandomStatArray = OptionPool->GetRandomOptionsWithInfos(ItemData.ItemGrade, OptionDetails);

			TArray<TPair<TSubclassOf<UGameplayEffect>, int32>, TFixedAllocator<MAX_ADDITIONAL_OPTION_COUNT>> AdditionalOptions;
			for (TPair RandomStat : RandomStatArray)
			{
				AdditionalOptions.Emplace(TPair<TSubclassOf<UGameplayEffect>, int32>(ICC->GetStatEffect(RandomStat.Key), RandomStat.Value));
			}
			EquipInfo->ChangeAdditionalOption(AdditionalOptions);

			EquipmentDataWidget->UpdateOptionData();
		}
	}
	
}
