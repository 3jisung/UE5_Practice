// Fill out your copyright notice in the Description page of Project Settings.


#include "MathGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMathGameMode::BeginPlay()
{
	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Main", AllActor);
}