// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PATROL.h"
#include "../Global/ARGlobal.h"
#include "PatrolPositions.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_PATROL::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::PATROL);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 200.0f;
	}

	int PatrolCount = UARGlobal::MainRandom.RandRange(4, 8);

	UPatrolPositions* PP = NewObject<UPatrolPositions>();
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("PatrolPositions"), PP);

	PP->CurrentIndex = 0;

	FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
	float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));

	for (size_t i = 0; i < PatrolCount; i++)
	{
		FVector RandomDir;
		RandomDir.X = UARGlobal::MainRandom.FRandRange(-100.0f, 100.0f);
		RandomDir.Y = UARGlobal::MainRandom.FRandRange(-100.0f, 100.0f);
		RandomDir.Normalize();
		RandomDir *= UARGlobal::MainRandom.FRandRange(SearchRange * 0.4f, SearchRange);
		RandomDir += OriginPos;
		PP->Positions.Add(RandomDir);
	}

	PP->Positions.Add(OriginPos);

	UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, PP->Positions[0]);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
	GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), 1);

	// 최초에 포지션을 여러개 만들어서 넣었다.
	// 여기에 넣었으므로 블랙보드 컴포넌트가 이 메모리를 지켜줄것이다를 생각하고 있다.

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_PATROL::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (true == IsDeathCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	UObject* PPObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("PatrolPositions"));

	if (nullptr == PPObject)
	{
		SetStateChange(OwnerComp, AIState::IDLE);
		return;
	}

	// 패트롤 최종지점에 도달했다면
	UPatrolPositions* PP = Cast<UPatrolPositions>(PPObject);
	TArray<FVector>& Positions = PP->Positions;
	int PatrolCurrentIndex = PP->CurrentIndex;
	if (PatrolCurrentIndex == Positions.Num())
	{
		// Idle로 다시 돌아간다.
		SetStateChange(OwnerComp, AIState::IDLE);
		return;
	}

	// 이제 일반적인 이동인데.
	UObject* NavObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (nullptr == NavPath)
	{
		// 갈수없는 곳을 찍었으므로
		// 그냥 다음패트롤 위치로 이동한다.
		++PP->CurrentIndex;
		return;
	}

	if (nullptr != NavPath && true == NavPath->PathPoints.IsEmpty())
	{
		// 갈수없는 곳을 찍었으므로
		// 그냥 다음패트롤 위치로 이동한다.
		++PP->CurrentIndex;
		return;
	}

	int CurrentIndex = GetBlackboardComponent(OwnerComp)->GetValueAsInt(TEXT("CurrentIndex"));

	FVector TargetPos = NavPath->PathPoints[CurrentIndex];
	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	AActor* ResultActor = GetTargetSearch(OwnerComp);

	// 타겟 발견 -> MOVE 상태로 전환
	if (nullptr != ResultActor)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
		SetStateChange(OwnerComp, AIState::MOVE);
		return;
	}

	// 타겟 없음 -> 순찰 계속 진행
	{
		TargetPos.Z = 0.0f;
		ThisPos.Z = 0.0f;

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
		FVector Dir = TargetPos - ThisPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);

		if (50.0f >= Dir.Size())
		{
			++CurrentIndex;

			if (NavPath->PathPoints.Num() <= CurrentIndex)
			{
				// 첫번째 패트롤 포인트까지 이상없이 이동했다는것.
				// 최종목적지까지 도달했다.
				// 그럼 다음 패트롤 포인트까지 변경한다.
				++PP->CurrentIndex;

				// 마지막 패트롤 포인트인 경우
				if (PatrolCurrentIndex == Positions.Num())
				{
					// Idle로 다시 돌아간다.
					SetStateChange(OwnerComp, AIState::IDLE);
					return;
				}

				// 에러가 날 가능성이 높은데 일단 그냥 진행하자.
				// 다음 패트롤 포인트로 변경 후 길찾기 오브젝트 최신화 및 CurrentIndex 1로 초기화
				UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, PP->Positions[PP->CurrentIndex]);
				GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
				GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), 1);
				return;
			}

			// 현재 패트롤 포인트의 다음 PathPoint로 이동
			GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), CurrentIndex);
			return;
		}
	}
}