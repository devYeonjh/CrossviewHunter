// Fill out your copyright notice in the Description page of Project Settings.


#include "CHStatusDisplayWidget.h"
#include "AbilitySystem/Attributes/CHStatSet.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Player/LyraPlayerState.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"


void UCHStatusDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCHStatusDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALyraPlayerState* LyraPS = PC->GetPlayerState<ALyraPlayerState>())
		{
			if (ULyraAbilitySystemComponent* LASC = LyraPS->GetLyraAbilitySystemComponent())
			{
				if (const UCHStatSet* StatSet = LASC->GetSet<UCHStatSet>())
				{
					BindToStatSet(StatSet);
				}
			}
		}
	}
}

void UCHStatusDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCHStatusDisplayWidget::BindToStatSet(const UCHStatSet* StatSet)                                                                                             
{
	UnbindFromStatSet(); // 기존 바인딩 해제                                                         

	BoundStatSet = StatSet;
	if (BoundStatSet)
	{
		// 각 속성 변경 델리게이트에 바인딩                                                          
		BoundStatSet->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
		BoundStatSet->OnAttackChanged.AddUObject(this, &ThisClass::OnAttackChanged);
		BoundStatSet->OnDefenceChanged.AddUObject(this, &ThisClass::OnDefenceChanged);

		UpdateAllStats(); // 초기값 표시                                                             
	}
}

void UCHStatusDisplayWidget::UnbindFromStatSet()
{
	
	
}

void UCHStatusDisplayWidget::OnHealthChanged(float NewValue)
{
	HealthText->SetText(FText::AsNumber(NewValue));
	UE_LOG(LogTemp, Log, TEXT("Health Changed : %f"), NewValue);
}

void UCHStatusDisplayWidget::OnAttackChanged(float NewValue)
{
	AttackText->SetText(FText::AsNumber(NewValue));
	UE_LOG(LogTemp, Log, TEXT("Attack Changed : %f"), NewValue);
}

void UCHStatusDisplayWidget::OnDefenceChanged(float NewValue)
{
	DefenceText->SetText(FText::AsNumber(NewValue));
	UE_LOG(LogTemp, Log, TEXT("Defence Changed : %f"), NewValue);
}

void UCHStatusDisplayWidget::UpdateAllStats()                                                        
{
	if (BoundStatSet)
	{
		HealthText->SetText(FText::AsNumber(BoundStatSet->GetHealth()));

		AttackText->SetText(FText::AsNumber(BoundStatSet->GetAttack()));

		DefenceText->SetText(FText::AsNumber(BoundStatSet->GetDefence()));
		
		CritRateText->SetText(FText::AsPercent(BoundStatSet->GetCritRate()));

		FireRateText->SetText(FText::AsNumber(BoundStatSet->GetFireDelay()));

		ReloadRateText->SetText(FText::AsNumber(BoundStatSet->GetReloadDelay()));

		ArmorPiercingDamageText->SetText(FText::AsNumber(BoundStatSet->GetArmorPiercingDamage()));

		CritDamageText->SetText(FText::AsNumber(BoundStatSet->GetCritDamage()));

		HeadDamageText->SetText(FText::AsNumber(BoundStatSet->GetHeadDamage()));
	}
}
