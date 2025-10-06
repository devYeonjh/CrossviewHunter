// Fill out your copyright notice in the Description page of Project Settings.

#include "CHDataTableImportHandler.h"
#include "CrossviewHunterEditor.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsSettings.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"
#include "Editor/UnrealEd/Public/Editor.h"

// FDelegateHandle FCHDataTableImportHandler::OnAssetPostImportHandle;
//
// void FCHDataTableImportHandler::Initialize()
// {
// 	if (GEditor)
// 	{
// 		OnAssetPostImportHandle = FEditorDelegates::OnAssetPostImport.AddStatic(&FCHDataTableImportHandler::OnAssetPostImport);
// 		UE_LOG(LogCrossviewHunterEditor, Log, TEXT("CHDataTableImportHandler initialized"));
// 	}
// }
//
// void FCHDataTableImportHandler::Shutdown()
// {
// 	if (OnAssetPostImportHandle.IsValid())
// 	{
// 		FEditorDelegates::OnAssetPostImport.Remove(OnAssetPostImportHandle);
// 		OnAssetPostImportHandle.Reset();
// 		UE_LOG(LogCrossviewHunterEditor, Log, TEXT("CHDataTableImportHandler shutdown"));
// 	}
// }
//
// void FCHDataTableImportHandler::OnAssetPostImport(UFactory* InFactory, UObject* InCreatedObject)
// {
// 	if (UDataTable* DataTable = Cast<UDataTable>(InCreatedObject))
// 	{
// 		ProcessDataTableGameplayTags(DataTable);
// 	}
// }
//
// bool FCHDataTableImportHandler::IsAllowedTagPrefix(const FString& TagString)
// {
// 	// 자동 생성을 허용할 태그 prefix 목록
// 	static const TArray<FString> AllowedPrefixes = {
// 		TEXT("CH.ID.Item."),
// 		TEXT("CH.ID.Grade."),
// 		TEXT("CH.ID.EquipmentType."),
// 		TEXT("CH.ID.EquipmentSlot."),
// 		TEXT("CH.ID.OptionPool."),
// 		TEXT("CH.ID.LootTable."),
// 		TEXT("CH.ID.LootGroup."),
// 	};
//
// 	for (const FString& Prefix : AllowedPrefixes)
// 	{
// 		if (TagString.StartsWith(Prefix))
// 		{
// 			return true;
// 		}
// 	}
//
// 	return false;
// }
//
// void FCHDataTableImportHandler::ProcessDataTableGameplayTags(UDataTable* DataTable)
// {
// 	if (!DataTable || !DataTable->GetRowStruct())
// 	{
// 		return;
// 	}
//
// 	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
// 	TSet<FName> NewTags;
//
// 	// DataTable의 모든 행을 순회
// 	for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
// 	{
// 		uint8* RowData = RowIt.Value();
//
// 		// 구조체의 모든 프로퍼티 검사
// 		for (TFieldIterator<FProperty> It(DataTable->GetRowStruct()); It; ++It)
// 		{
// 			FProperty* Property = *It;
//
// 			// FGameplayTag 타입 확인
// 			if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
// 			{
// 				if (StructProp->Struct == FGameplayTag::StaticStruct())
// 				{
// 					FGameplayTag* TagPtr = StructProp->ContainerPtrToValuePtr<FGameplayTag>(RowData);
//
// 					if (TagPtr && TagPtr->IsValid())
// 					{
// 						// 태그가 존재하지 않으면 추가 대상으로 등록
// 						FGameplayTag ExistingTag = TagManager.RequestGameplayTag(TagPtr->GetTagName(), false);
// 						if (!ExistingTag.IsValid())
// 						{
// 							NewTags.Add(TagPtr->GetTagName());
// 						}
// 					}
// 				}
// 				// FGameplayTagContainer도 처리
// 				else if (StructProp->Struct == FGameplayTagContainer::StaticStruct())
// 				{
// 					FGameplayTagContainer* ContainerPtr = StructProp->ContainerPtrToValuePtr<FGameplayTagContainer>(RowData);
//
// 					if (ContainerPtr)
// 					{
// 						for (const FGameplayTag& Tag : *ContainerPtr)
// 						{
// 							if (Tag.IsValid())
// 							{
// 								FGameplayTag ExistingTag = TagManager.RequestGameplayTag(Tag.GetTagName(), false);
// 								if (!ExistingTag.IsValid())
// 								{
// 									NewTags.Add(Tag.GetTagName());
// 								}
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
//
// 	// 새 태그들을 GameplayTags.ini에 추가
// 	if (NewTags.Num() > 0)
// 	{
// 		UGameplayTagsSettings* Settings = GetMutableDefault<UGameplayTagsSettings>();
// 		int32 AddedCount = 0;
//
// 		for (const FName& TagName : NewTags)
// 		{
// 			FString TagString = TagName.ToString();
//
// 			// prefix 검증
// 			if (IsAllowedTagPrefix(TagString))
// 			{
// 				FGameplayTagTableRow NewRow;
// 				NewRow.Tag = TagName;
// 				NewRow.DevComment = FString::Printf(TEXT("Auto-generated from DataTable: %s"), *DataTable->GetName());
//
// 				Settings->GameplayTagTableList.Add(NewRow);
// 				AddedCount++;
//
// 				UE_LOG(LogCrossviewHunterEditor, Display, TEXT("Auto-added GameplayTag: %s from %s"),
// 					*TagString, *DataTable->GetName());
// 			}
// 			else
// 			{
// 				UE_LOG(LogCrossviewHunterEditor, Warning,
// 					TEXT("Tag '%s' in DataTable '%s' is not allowed for auto-generation. Please add it manually or check the tag prefix."),
// 					*TagString, *DataTable->GetName());
// 			}
// 		}
//
// 		// 태그를 추가했다면 설정 저장 및 태그 매니저 리프레시
// 		if (AddedCount > 0)
// 		{
// 			Settings->SaveConfig();
// 			TagManager.EditorRefreshGameplayTagTree();
//
// 			UE_LOG(LogCrossviewHunterEditor, Display, TEXT("Added %d new GameplayTag(s) from DataTable: %s"),
// 				AddedCount, *DataTable->GetName());
// 		}
// 	}
// }
