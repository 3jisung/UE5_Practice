// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGlobalCharacter::AGlobalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGlobalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GlobalAnimInstance = Cast<UGlobalAnimInstance>(GetMesh()->GetAnimInstance());

	GlobalAnimInstance->AllAnimations = AllAnimations;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGlobalCharacter::BeginOverLap);
}

// Called every frame
void AGlobalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGlobalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGlobalCharacter::BeginOverLap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherComp->ComponentHasTag(TEXT("damage")))
	{
		if (HitState)
		{
			HP -= 20;
			//UE_LOG(LogTemp, Error, TEXT("test"));
			SetHitState();

			if (HP < 0)
			{
				HP = 0;
			}
		}
	}
}

void AGlobalCharacter::SetHitState()
{
	HitState = false;
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 내가 원하는 코드 구현
			HitState = true;

			// 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
		}), HitIgnoreTime, false); // 반복 실행을 하고 싶으면 false 대신 true 대입
}