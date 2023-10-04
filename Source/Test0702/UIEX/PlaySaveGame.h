// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlaySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API UPlaySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector PlayerPos;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator PlayerRot;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector PlayerScale;

	UFUNCTION(BlueprintCallable, Category = "ARGlobal", meta = (DefaultToSelf = "_Object"))
	void DataSave(ACharacter* _World);

	UFUNCTION(BlueprintCallable, Category = "ARGlobal", meta = (DefaultToSelf = "_Object"))
	void DataLoad(ACharacter* _World);
};
