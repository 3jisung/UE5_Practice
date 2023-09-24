// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUserWidget.h"
#include "../Global/GlobalGameInstance.h"
#include "InvenItemSlot.h"
#include "InvenItemData.h"

// 굉장히 위험도가 높다.
UInventoryUserWidget* UInventoryUserWidget::InvenWidget = nullptr;

// GameMode아니면 HUD가 알게하는게 가장 좋습니다.

// 데이터와 랜더링은 분리되어야 한다.
void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InvenWidget = this;

	InvenList = Cast<UTileView>(GetWidgetFromName(TEXT("InvenTileView")));

	// 여기에다가 뭘 넣어줘야 하는지가 내일 배워야할 내용.

	for (size_t i = 0; i < 20; i++)
	{
		UInvenItemData* ItemObject = NewObject<UInvenItemData>();
		ItemObject->Data = nullptr;
		ItemObject->Index = i;
		InvenList->AddItem(ItemObject);
	}

	UGlobalGameInstance* GameInstance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const TArray<UObject*>& Items = InvenList->GetListItems();
	for (size_t i = 0; i < 3; i++)
	{
		UInvenItemData* DataObject = Cast<UInvenItemData>(Items[i]);
		DataObject->Data = GameInstance->GetRandomItemData();
	}
}

void UInventoryUserWidget::AddInvenItem(UObject* _Data, UUserWidget* _Widget)
{
	UInvenItemSlot* ItemSlotWidget = Cast<UInvenItemSlot>(_Widget);

	UInvenItemData* InvenSlotData = Cast<UInvenItemData>(_Data);

	if (nullptr == ItemSlotWidget)
	{
		return;
	}

	InvenSlotData->Widget = ItemSlotWidget;

	UImage* Select = Cast<UImage>(ItemSlotWidget->GetWidgetFromName(TEXT("Select")));

	if (InvenSlotData->Index != 0)
	{
		Select->SetVisibility(ESlateVisibility::Hidden);
	}

	ItemSlotWidget->SetItemData(InvenSlotData);
}