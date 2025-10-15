// Fill out your copyright notice in the Description page of Project Settings.

#include "CHOptionPoolEditorUtility.h"
#include "Item/CHOptionPool.h"
#include "Editor.h"

void FCHOptionPoolEditorUtility::HandlePoolIDChanged(UCHOptionPool* OptionPool)
{
	if (!OptionPool)
	{
		return;
	}

	// OptionPool의 LoadMatchingOptionsFromDataTable 호출
	// 이 함수는 public이어야 하거나, OptionPool에서 friend 선언이 필요함
	// 현재는 public 함수로 가정
}

void FCHOptionPoolEditorUtility::RefreshEditorViewports()
{
	if (GEditor)
	{
		GEditor->RedrawAllViewports();
	}
}

void FCHOptionPoolEditorUtility::TriggerPropertyChangeEvent(UCHOptionPool* OptionPool)
{
	if (!OptionPool || !GEditor)
	{
		return;
	}

	// OptionDetails 프로퍼티 변경 이벤트 생성 및 트리거
	FPropertyChangedEvent PropertyEvent(
		FindFieldChecked<FProperty>(OptionPool->GetClass(), TEXT("OptionDetails"))
	);

	OptionPool->PostEditChangeProperty(PropertyEvent);
}
