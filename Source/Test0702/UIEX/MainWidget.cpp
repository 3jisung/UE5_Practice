// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Inven = Cast<UInventoryUserWidget>(GetWidgetFromName(TEXT("WBP_Inventory")));
	// Inven = Cast<UInventoryUserWidget>(GetWidgetFromName(TEXT("WBP_Status")));

	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_Inventory"))));
	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_Status"))));
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 기본적인 이동이나 이런 시스템자체가 다르다는것을 기억해야 한다.
	// Inven->SetPositionInViewport({100, 100});
	// Inven->SetAnchorsInViewport();
}

// 여기서 리턴하는 bool 값은 지금 위젯의 상태가 바뀌었는지를 체크하는 bool 값이 된다.
bool UMainWidget::CheckWidGetUpdate()
{
	bool AllHiddenCheck = true;

	for (size_t i = 0; i < AllWidGet.Num(); i++)
	{
		if (AllWidGet[i]->GetVisibility() == ESlateVisibility::Visible)
		{
			AllHiddenCheck = false;
		}
	}

	// false였는데 true가 되고
	// true였는데 false가 되는 상황이 오면
	if (AllHiddenCheck != AllHidden)
	{
		AllHidden = AllHiddenCheck;
		return true;
	}

	return false;
}