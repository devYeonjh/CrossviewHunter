// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

/**
 * 
 */

class CROSSVIEWHUNTEREDITOR_API FCHMonsterSpawnPointRowCustomization : public IPropertyTypeCustomization 
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
 
	// 헤더 커스터마이즈 (배열 요소 이름 표시)
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
 
	// 자식 프로퍼티 커스터마이즈(각 프로퍼티 변경 델리게이트 바인딩)
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};
