// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

/**
 * UCHOptionPool에 대한 에디터 전용 유틸리티 함수들
 *
 * PostEditChangeProperty 로직과 에디터 UI 새로고침 등
 * 런타임 모듈에 포함되면 안 되는 에디터 기능들을 제공합니다.
 */
class CROSSVIEWHUNTEREDITOR_API FCHOptionPoolEditorUtility
{
public:
	/**
	 * OptionPool의 PoolID 변경 시 DataTable에서 매칭되는 옵션들을 로드합니다.
	 *
	 * @param OptionPool 업데이트할 OptionPool 객체
	 */
	static void HandlePoolIDChanged(class UCHOptionPool* OptionPool);

	/**
	 * 에디터 뷰포트를 새로고침합니다.
	 */
	static void RefreshEditorViewports();

	/**
	 * OptionPool의 OptionDetails 프로퍼티 변경 이벤트를 트리거합니다.
	 *
	 * @param OptionPool 이벤트를 트리거할 OptionPool 객체
	 */
	static void TriggerPropertyChangeEvent(class UCHOptionPool* OptionPool);
};
