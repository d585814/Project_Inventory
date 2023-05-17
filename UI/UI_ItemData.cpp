// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemData.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "UI_ItemTooltip.h"



void UUI_ItemData::NativeConstruct()
{
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(Item.Item.RowName, "");
	if (OutRow == nullptr) return;
	ItemData = *OutRow;

	//Set Tooltip Widget
	if (ItemTooltipClass == NULL) return;
	UUI_ItemTooltip* Tooltip = CreateWidget<UUI_ItemTooltip>(GetOwningPlayer(), ItemTooltipClass);

	if (Tooltip == nullptr) return;
	Tooltip->SInv = ItemData;
	Tooltip->Amount = Item.Amount;

	if (ItemDataP == nullptr) return;
	ItemDataP->SetToolTip(Tooltip);
}

bool UUI_ItemData::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (ItemDataP==nullptr) return false;
	ItemDataP->BackgroundDelegate.BindDynamic(this, &UUI_ItemData::MakeBrush);

	

	return true;
}

FReply UUI_ItemData::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return ReplyResult.NativeReply;
}

void UUI_ItemData::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	

	if (DragDropOperClass == NULL) return;
	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperClass));
	if (Oper == nullptr) return;
	FSlot OperItem;
	OperItem.Item = Item.Item;
	OperItem.Amount = 1;
	
	// Set ItemDragDropOperation
	Oper->Pivot = EDragPivot::CenterCenter;
	Oper->Tag = Source;
	Oper->DefaultDragVisual = this;
	Oper->Item = OperItem;
	Oper->Index = Index;	
	

	OutOperation = Oper;
}

void UUI_ItemData::CreatedSetting(FSlot NewItem, int32 NewIndex, FString NewSource)
{
	Item = NewItem;
	Index = NewIndex;
	Source = NewSource;

	FName LocalRowName = Item.Item.RowName;
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName, "");

	if (OutRow == nullptr) return;
	ItemData = *OutRow;
	
}


FSlateBrush UUI_ItemData::MakeBrush()
{
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemData.Icon, 0, 0);

	return Brush;
}

