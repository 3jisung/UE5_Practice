// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Global/Data/ItemData.h"
#include "InvenItemData.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API UInvenItemData : public UObject
{
	GENERATED_BODY()
	
public:
	// 실제 위젯
	class UInvenItemSlot* Widget;
	int Index = 0;
	int Count = 1;
	const FItemData* Data;
};
