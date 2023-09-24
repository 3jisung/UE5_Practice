// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalAnimInstance.h"
#include "GlobalCharacter.h"

void UGlobalAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageBlendingOut.AddDynamic(this, &UGlobalAnimInstance::MontageEnd);
}

// 시작할때 한번 업데이트를 돌려주므로.
void UGlobalAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == Character && false == Character->IsValidLowLevel())
	{
		return;
	}

	AniState = Character->GetAniState();

	if (AllAnimations.Find(AniState) == nullptr)
	{
		return;
	}

	class UAnimMontage* Montage = AllAnimations[AniState];

	if (nullptr == Montage)
	{
		return;
	}

	// 조건1. 몽타주 재생이 끝났다.
	// 조건2. 이전에 이미 재생한 몽타주다.
	// 조건3. 몽타주가 루프다.
	if (false == Montage_IsPlaying(Montage))
	{
		//if (Montage == CurMontage && -1 != DefaultAniState && false == Montage->bLoop)
		//{
		//	Montage = AllAnimations[DefaultAniState];
		//}

		CurMontage = Montage;
		Montage_Play(Montage, 1.0f);
	}
}

void UGlobalAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{

}