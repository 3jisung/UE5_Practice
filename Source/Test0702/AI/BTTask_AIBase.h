// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../Global/GlobalCharacter.h"
#include "../Global/GlobalEnums.h"
#include "AICon.h"
#include "BTTask_AIBase.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AIBase();
	void OnGameplayTaskActivated(class UGameplayTask&) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds) override;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	class AGlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);
	class UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);

	float GetStateTime(UBehaviorTreeComponent& OwnerComp);
	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);

	AIState GetAiState(UBehaviorTreeComponent& OwnerComp);

	class AActor* GetTargetSearch(UBehaviorTreeComponent& OwnerComp);

	template<typename EnumType>
	void SetStateChange(UBehaviorTreeComponent& OwnerComp, EnumType _State)
	{
		SetStateChange(OwnerComp, static_cast<uint8>(_State));
	}

	void SetStateChange(UBehaviorTreeComponent& OwnerComp, uint8 _State);

	bool IsDeathCheck(UBehaviorTreeComponent& OwnerComp);
};
