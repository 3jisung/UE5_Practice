// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "../Global/GlobalGameInstance.h"
#include "../Global/GlobalCharacter.h"
#include "../Global/ARGlobal.h"
#include "../UIEX/MiniMapComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AMonster::AMonster()
{
	//PrimaryActorTick.bCanEverTick = true;

	MiniMap = CreateDefaultSubobject<UMiniMapComponent>(TEXT("MiniMapComponent"));
	MiniMap->SetupAttachment(RootComponent);
	MiniMap->MiniMapInit(this);
}


void AMonster::BeginPlay()
{
	// 콜백은 Player의 함수가 추가될수가 있는것이다.
	// OnDestroyed.AddDynamic(this, &AProjectTile::DestroyProjectile);

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);

		SetAllAnimation(CurMonsterData->MapAnimation);
		SetAniState(AIState::DEATH);
	}

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(AIState::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1500.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 200.0f);
	FVector Pos = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), Pos);
}

void AMonster::Tick(float DeltaTime)
{
	UARGlobal::ARDebugTextPrint(this, FString::FromInt(GetAniState()));
}