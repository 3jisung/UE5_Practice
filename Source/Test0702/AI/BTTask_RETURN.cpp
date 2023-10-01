// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RETURN.h"

EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RETURN);

	// 돌아가야할 위치는

	FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));

	UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, OriginPos);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
	GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), 1);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (true == IsDeathCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	UObject* NavObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (nullptr == NavPath)
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	if (nullptr != NavPath && 1 >= NavPath->PathPoints.Num())
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	int CurrentIndex = GetBlackboardComponent(OwnerComp)->GetValueAsInt(TEXT("CurrentIndex"));

	// 문제가 될수 있다.
	// 1번 찾은 경로를 완전히 끝까지 돌아가는것이 중요해졌다.
	FVector TargetPos = NavPath->PathPoints[CurrentIndex];
	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;

	{
		// 혹시라도 z축이 있을 가능성을 없애는게 보통입니다.

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DeltaSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}

	{
		ThisPos.Z = 0.0f;
		TargetPos.Z = 0.0f;

		FVector Dir = TargetPos - ThisPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);

		if (10.0f >= Dir.Size())
		{
			++CurrentIndex;

			if (NavPath->PathPoints.Num() <= CurrentIndex)
			{
				// 최종목적지까지 도달했다.
				SetStateChange(OwnerComp, AIState::IDLE);
				return;
			}

			GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), CurrentIndex);
			return;
		}
	}
}