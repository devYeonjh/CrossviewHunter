// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * DataTable CSV Import 시 GameplayTag 자동 생성 핸들러
 *
 * CSV import 후 FGameplayTag 필드에 존재하지 않는 태그가 있으면
 * 지정된 prefix를 가진 경우 자동으로 GameplayTagTableList에 추가합니다.
 */
class CROSSVIEWHUNTEREDITOR_API FCHDataTableImportHandler
{
	
// public:
// 	/** 핸들러 초기화 - 에디터 모듈 StartupModule에서 호출 */
// 	static void Initialize();
//
// 	/** 핸들러 종료 - 에디터 모듈 ShutdownModule에서 호출 */
// 	static void Shutdown();
//
// private:
// 	/** Asset Import 후 콜백 */
// 	static void OnAssetPostImport(UFactory* InFactory, UObject* InCreatedObject);
//
// 	/** DataTable의 GameplayTag 필드 처리 */
// 	static void ProcessDataTableGameplayTags(UDataTable* DataTable);
//
// 	/** 태그가 자동 생성 허용 prefix인지 확인 */
// 	static bool IsAllowedTagPrefix(const FString& TagString);
//
// 	static FDelegateHandle OnAssetPostImportHandle;
};
