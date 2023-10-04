// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Test.h"

void UAnimNotifyState_Test::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Error, TEXT("NotifyBegin"));
}
void UAnimNotifyState_Test::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Error, TEXT("NotifyTick"));
}
void UAnimNotifyState_Test::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Error, TEXT("NotifyEnd"));
}