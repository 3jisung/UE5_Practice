// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMapActor.generated.h"

UCLASS()
class TEST0702_API AMiniMapActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMiniMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* TraceActor = nullptr;
};
