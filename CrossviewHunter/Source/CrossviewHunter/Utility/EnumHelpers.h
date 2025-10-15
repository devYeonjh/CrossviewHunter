// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnumHelpers.generated.h"

/**
 * 
 */
UCLASS()
class CROSSVIEWHUNTER_API UEnumHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// EnumType의 이름을 FName으로 반환하는 함수
	template<typename T>
	static FName GetEnumFName(T EnumType)
	{
		const UEnum* EnumClass = StaticEnum<T>();
		check(EnumClass != nullptr);

		const FString EnumName = EnumClass->GetNameStringByValue(static_cast<int64>(EnumType));
		return FName(*EnumName);
	}

	template<typename T>
	static FText GetEnumDisplayName(T EnumType)
	{
		const UEnum* EnumClass = StaticEnum<T>();
		check(EnumClass != nullptr);
 
		return EnumClass->GetDisplayNameTextByValue(static_cast<int64>(EnumType));
	}
};