// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class AIState : uint8
{
	NONE UMETA(DisplayName = "사용안함"),
	IDLE UMETA(DisplayName = "대기"),
	MOVE UMETA(DisplayName = "이동"),
	RETURN UMETA(DisplayName = "복귀"),
	PATROL UMETA(DisplayName = "순찰"),
	ATTACK UMETA(DisplayName = "공격"),
	DEATH UMETA(DisplayName = "죽음"),
};

UENUM(BlueprintType)
enum class ItemType : uint8
{
	NONE UMETA(DisplayName = "쓰면바보"),
	ARMOR UMETA(DisplayName = "갑옷"),
	WEAPON UMETA(DisplayName = "무기"),
	POSTION UMETA(DisplayName = "포션"),
	QUEST UMETA(DisplayName = "퀘스트"),
};
