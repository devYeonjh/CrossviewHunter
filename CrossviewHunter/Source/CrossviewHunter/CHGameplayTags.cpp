// Fill out your copyright notice in the Description page of Project Settings.


#include "CHGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace CHGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Health, "SetByCaller.Health", "SetByCaller tag used by health gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Attack, "SetByCaller.Attack", "SetByCaller tag used by Attack gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Defence, "SetByCaller.Defence", "SetByCaller tag used by Defense gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_CritRate, "SetByCaller.CritRate", "SetByCaller tag used by CritRate gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_MoveSpeed, "SetByCaller.MoveSpeed", "SetByCaller tag used by MoveSpeed gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_FireDelay, "SetByCaller.FireDelay", "SetByCaller tag used by FireDelay gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_ReloadDelay, "SetByCaller.ReloadDelay", "SetByCaller tag used by ReloadDelay gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_ArmorPiercingDamage, "SetByCaller.ArmorPiercingDamage", "SetByCaller tag used by ArmorPiercingDamage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_CritDamage, "SetByCaller.CritDamage", "SetByCaller tag used by CritDamage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_HeadDamage, "SetByCaller.HeadDamage", "SetByCaller tag used by HeadDamage gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_DT_ItemDataTable, "CH.DT.ItemDataTable", "ItemDataTable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_DT_EquipmentTypeDataTable, "CH.DT.EquipmentTypeDataTable", "EquipmentTypeDataTable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_DT_LootTable, "CH.DT.LootTable", "LootTable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_DT_LootGroup, "CH.DT.LootGroup", "LootGroup");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_Item, "CH.ID.Item", "Item");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_Grade, "CH.ID.Grade", "Item Grade");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_EquipmentType, "CH.ID.EquipmentType", "EquipmentType");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_EquipmentSlot, "CH.ID.EquipmentSlot", "EquipmentSlot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_OptionPool, "CH.ID.OptionPool", "OptionPool");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_LootTable, "CH.ID.LootTable", "LootTable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CH_ID_LootGroup, "CH.ID.LootGroup", "LootGroup");
	
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}

