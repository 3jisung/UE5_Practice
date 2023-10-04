// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_GlobalSound.h"
#include "Components/AudioComponent.h"
#include "GlobalCharacter.h"

void UAnimNotify_GlobalSound::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	AGlobalCharacter* Ptr = Cast<AGlobalCharacter>(MeshComp->GetOwner());

	if (nullptr == Ptr)
	{
		return;
	}

	UAudioComponent* Audio = Cast<UAudioComponent>(Ptr->GetComponentByClass(UAudioComponent::StaticClass()));

	if (nullptr == Audio)
	{
		return;
	}

	USoundBase* Sound = Ptr->GetSound(SoundIndex);

	if (nullptr != Sound)
	{
		Audio->SetSound(Sound);
		Audio->Play();
	}
}