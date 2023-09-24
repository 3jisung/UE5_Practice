// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenItemSlot.h"

void UInvenItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIconBack = Cast<UImage>(GetWidgetFromName(TEXT("ItemBack")));
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));

	// 인벤토리 아이템에 있는 UInvenItemData는 인벤토리라면 무조건 가지고 있어야 합니다.
	ItemData = NewObject<UInvenItemData>();
}

void UInvenItemSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UInvenItemSlot::SetItemData(UInvenItemData* _ItemData)
{
	ItemData = _ItemData;
	SlotDataCheck();
}

void UInvenItemSlot::SlotDataCheck()
{
	if (nullptr == ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == ItemData)"), __FUNCTION__, __LINE__);
		// 치명적인 에러
		//ItemBackVisibility = ESlateVisibility::Visible;
		//ItemIconVisibility = ESlateVisibility::Hidden;
		//ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr == ItemData->Data)
	{
		ItemBackVisibility = ESlateVisibility::Visible;
		ItemIconVisibility = ESlateVisibility::Hidden;
		ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr != ItemData->Data)
	{
		ItemIconVisibility = ESlateVisibility::Visible;
		ItemIconImage->SetBrushFromTexture(Cast<UTexture2D>(ItemData->Data->Icon));
		ItemCountValue = ItemData->Count;

		if (1 < ItemData->Data->StackMax)
		{
			ItemCountVisibility = ESlateVisibility::Visible;
		}
		else
		{
			ItemCountVisibility = ESlateVisibility::Hidden;
		}
	}
}