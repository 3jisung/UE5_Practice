// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_AIBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PATROL.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API UBTTask_PATROL : public UBTTask_AIBase
{
	GENERATED_BODY()

protected:

	//UPROPERTY()
	//UPatrolPositions* PP;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds);
};
