// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_AIBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ATTACK.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API UBTTask_ATTACK : public UBTTask_AIBase
{
	GENERATED_BODY()

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds);
};
