// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../UIEX/MainWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEST0702_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();
	~AGameHUD();

	UFUNCTION(BlueprintCallable)
	UMainWidget* GetMainWidget()
	{
		return MainWidget;
	}

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

private:
	// TSubclassOf<UUserWidget> MainWidgetClass = nullptr;

	UMainWidget* MainWidget = nullptr;
};
