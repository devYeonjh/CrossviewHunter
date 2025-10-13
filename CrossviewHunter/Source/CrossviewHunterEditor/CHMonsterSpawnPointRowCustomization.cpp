// Fill out your copyright notice in the Description page of Project Settings.


#include "CHMonsterSpawnPointRowCustomization.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "CHPhaseDataEditorUtilityWidget.h"
#include "Engine/Engine.h"



TSharedRef<IPropertyTypeCustomization> FCHMonsterSpawnPointRowCustomization::MakeInstance()
{
	return MakeShareable(new FCHMonsterSpawnPointRowCustomization);
}

void FCHMonsterSpawnPointRowCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// 배열 요소 이름 표시 (예: SpawnPointRowsData[3])
	int32 ArrayIndex = PropertyHandle->GetIndexInArray();
	FString DisplayName = (ArrayIndex != INDEX_NONE) ? FString::Printf(TEXT("SpawnPointRowsData[%d]"), ArrayIndex) : TEXT("SpawnPointRow");
 
	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget(FText::FromString(DisplayName))
		];
}

void FCHMonsterSpawnPointRowCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren = 0;
	PropertyHandle->GetNumChildren(NumChildren);
 
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		TSharedPtr<IPropertyHandle> ChildHandle = PropertyHandle->GetChildHandle(ChildIndex);
        if (ChildHandle.IsValid())
        {
            // FVector 타입이면 내부( X, Y, Z ) 각각에 델리게이트 등록
            if (ChildHandle->GetProperty() && ChildHandle->GetProperty()->GetCPPType() == TEXT("FVector"))
            {
                uint32 NumVectorChildren = 0;
                ChildHandle->GetNumChildren(NumVectorChildren);
 
                for (uint32 VecChildIndex = 0; VecChildIndex < NumVectorChildren; ++VecChildIndex)
                {
                    TSharedPtr<IPropertyHandle> VectorComponentHandle = ChildHandle->GetChildHandle(VecChildIndex);
                    if (VectorComponentHandle.IsValid())
                    {
                        VectorComponentHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([ChildHandle, PropertyHandle]()
                        {
                            int32 ArrayIndex = PropertyHandle->GetIndexInArray();
                            FString PropertyName = ChildHandle->GetProperty() ? ChildHandle->GetProperty()->GetName() : TEXT("Unknown");
 
                            if (GEditor)
                            {
                                UCHRowPropertySubsystem* Subsystem = GEditor->GetEditorSubsystem<UCHRowPropertySubsystem>();
                                if (Subsystem)
                                {
                                    Subsystem->BroadcastSpawnPointRowChanged(ArrayIndex, FName(PropertyName));
                                }
                            }
                        }));
                    }
                }
            }
            else
            {
                // FVector 타입이 아니면 기존처럼 델리게이트 등록
                ChildHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([ChildHandle, PropertyHandle]()
                {
                    int32 ArrayIndex = PropertyHandle->GetIndexInArray();
                    FString PropertyName = ChildHandle->GetProperty() ? ChildHandle->GetProperty()->GetName() : TEXT("Unknown");
 
                    if (GEditor)
                    {
                        UCHRowPropertySubsystem* Subsystem = GEditor->GetEditorSubsystem<UCHRowPropertySubsystem>();
                        if (Subsystem)
                        {
                            Subsystem->BroadcastSpawnPointRowChanged(ArrayIndex, FName(PropertyName));
                        }
                    }
                }));
            }
 
            ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
        }
	}
}
