// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MOVE.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_MOVE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::MOVE);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 500.0f;
	}

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();

	UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, TargetPos);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
	GetBlackboardComponent(OwnerComp)->SetValueAsVector(TEXT("LastPos"), TargetPos);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_MOVE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr == TargetActor)
	{
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	if (true == IsDeathCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	// 너무 잦은 횟수로 길을 찾는 경우를 막는 경우는 다음과 같은 방법이 있다.
	// 길을 너무 많이 찾으면 나중에 

	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector TargetPos = TargetActor->GetActorLocation();	// 타겟까지 가는 길의 PathPoints[CurrentIndex] 가 들어갈 값
	FVector LastPos = TargetActor->GetActorLocation();		// 타겟의 현재 위치
	FVector CheckLastPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("LastPos"));

	if (LastPos != CheckLastPos)
	{
		// 타겟이 움직였을 경우에만 새로 갱신
		UNavigationPath* NewPath = PathFindNavPath(OwnerComp, LastPos);
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), NewPath);
	}

	// 이미 존재하기 때문에 그냥 이미 존재하는 것에서 찾아오면 된다.
	UObject* NavObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (nullptr == NavPath)
	{
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	// 길을 제대로 못찾았다는 이야기가 된다.
	if (nullptr != NavPath && true == NavPath->PathPoints.IsEmpty())
	{
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	if (nullptr != NavPath)
	{
		TargetPos = NavPath->PathPoints[1];
	}

	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;
	LastPos.Z = 0.0f;

	{
		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		// 타겟과의 방향이 10.0f 이상 어긋날 경우 방향 조정
		if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DeltaSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		// 그 외에는 방향값 그대로 유지
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}

	{
		FVector Dir = TargetPos - ThisPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);
		//GetGlobalCharacter(OwnerComp)->SetActorRotation(Dir.Rotation());

		FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		OriginPos.Z = 0.0f;
		FVector OriginDir = OriginPos - ThisPos;

		// 원래 위치에서 탐색 범위의 1.5배보다 멀어지면 다시 복귀
		if (OriginDir.Size() >= SearchRange * 1.5f)
		{
			GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
			SetStateChange(OwnerComp, AIState::RETURN);
			int a = 0;
			return;
		}

		FVector LastDir = LastPos - ThisPos;

		// 타겟이 탐색 범위보다 멀어질 경우 그 자리에서 대기
		if (SearchRange < LastDir.Size())
		{
			SetStateChange(OwnerComp, AIState::IDLE);
			return;
		}

		// 공격 범위까지 근접
		if (AttackRange >= LastDir.Size())
		{
			SetStateChange(OwnerComp, AIState::ATTACK);
			return;
		}
	}
}