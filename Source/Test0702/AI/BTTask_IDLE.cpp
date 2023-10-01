// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IDLE.h"
#include "../Global/GlobalEnums.h"

EBTNodeResult::Type UBTTask_IDLE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::IDLE);

	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_IDLE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (true == IsDeathCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (2.0f <= GetStateTime(OwnerComp))
	{
		// 지역변수로 랜덤을 이용하는것.
		FRandomStream Stream;
		// 알아서 시간과 같은 변하는 수를 시드값으로 삼는다.
		Stream.GenerateNewSeed();

		int Value = Stream.RandRange(0, 1);

		// UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, Value);

		ResetStateTime(OwnerComp);

		//if (Value == 0)
		//{
		SetStateChange(OwnerComp, AIState::PATROL);
		return;
		//}


		// 그럼 순찰할까?
		// 그냥 5초간 다시 대기하자.
		// SetStateChange(OwnerComp, AIState::MOVE);
		// return;
	}

	AActor* ResultActor = GetTargetSearch(OwnerComp);

	if (nullptr != ResultActor)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
		SetStateChange(OwnerComp, AIState::MOVE);
		return;
	}

	//핵심은 플레이어를 찾아내는것.
	// GetGlobalCharacter(OwnerComp)->GetLevel()->;

	return;
}