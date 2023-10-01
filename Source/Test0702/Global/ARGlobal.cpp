// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGlobal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "GlobalDebugUserWidget.h"

class UGlobalDebugUserWidget* UARGlobal::DebugWidget = nullptr;
FRandomStream UARGlobal::MainRandom;

UWorld* UARGlobal::GetCurrentWorld()
{
	// 언리얼 엔진 그자체에 대한 기능을 제공하는 포인터
	// GEngine->GetWorld();

	// 월드가 동시에 여러개가 존재할때가 있다.

	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		EWorldType::Type GetWorldType = Context.WorldType;

		if (nullptr == Context.World())
		{
			continue;
		}

		FString Name = Context.World()->GetName();

		// UE_LOG(LogTemp, Error, TEXT("%s"), *Name);

		switch (GetWorldType)
		{
		case EWorldType::None:
			break;
		case EWorldType::Game:
			break;
		case EWorldType::Editor:
			break;
		case EWorldType::PIE:
			return Context.World();
		case EWorldType::EditorPreview:
			break;
		case EWorldType::GamePreview:
			break;
		case EWorldType::GameRPC:
		case EWorldType::Inactive:
			break;
		default:
			break;
		}
	}

	return nullptr;
}

void UARGlobal::ARDebugTextInit()
{
	// 끝날을때 자동으로 지워지지만
	// delete를 한다고 
	DebugWidget = nullptr;
}

void UARGlobal::ARDebugTextPrint(class AActor* _AActor, const FString& _Text)
{
	// 여기서 만약 위젯이 없다면
	// 만들어 내야 한다.
	// 위젯이 없어.
	if (nullptr == DebugWidget)
	{
		UWorld* World = _AActor->GetWorld();

		if (nullptr == World && false == World->IsValidLowLevel())
		{
			return;
		}

		// 

		// 이걸 런타임 중간에 알아내는 방법은

		FSoftClassPath ObjectClass("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Global/WBP_GlobalDebugUserWidget.WBP_GlobalDebugUserWidget_C'");
		
		TSubclassOf<UUserWidget> SubClassValue = ObjectClass.TryLoadClass<UUserWidget>();

		if (nullptr == SubClassValue)
		{
			return;
		}

		UUserWidget* NewWidGet = CreateWidget<UUserWidget>(World, SubClassValue);

		if (nullptr == NewWidGet)
		{
			// 생성에 실패할때까 있을수 있으므로 이것도 처리해준다.
			return;
		}

		DebugWidget = Cast<UGlobalDebugUserWidget>(NewWidGet);
		DebugWidget->AddToViewport(1000);
	}
	DebugWidget->AddDebugText(_Text);
}