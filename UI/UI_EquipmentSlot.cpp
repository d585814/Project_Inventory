// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipmentSlot.h"
#include "MyInven/MyInvenCharacter.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI_ItemTooltip.h"
#include "ItemDragDropOperation.h"


void UUI_EquipmentSlot::NativeConstruct()
{
	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Cha == nullptr) return;

	Item = Cha->Equipment[SlotIndex];

	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(Item.Item.RowName, "");
	if (OutRow == nullptr) return;

	ItemData = *OutRow;

	if (ItemTooltipClass == NULL) return;
	UUI_ItemTooltip* Tooltip = CreateWidget<UUI_ItemTooltip>(GetOwningPlayer(), ItemTooltipClass);

	if (Tooltip == nullptr) return;
	Tooltip->SInv = ItemData;
	Tooltip->Amount = Item.Amount;

	if (EquipBorder == nullptr) return;
	EquipBorder->SetToolTip(Tooltip);
}

bool UUI_EquipmentSlot::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (EquipBorder == nullptr) return false;
	EquipBorder->BackgroundDelegate.BindDynamic(this, &UUI_EquipmentSlot::MakeBrush);



	return true;
}

FReply UUI_EquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;
}

void UUI_EquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
	Oper->Tag = "Equipment";
	Oper->DefaultDragVisual = this;
	Oper->Item = OperItem;
	Oper->Index = SlotIndex;


	OutOperation = Oper;
}

bool UUI_EquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(InOperation);
	if (Oper==nullptr) return true;

	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Cha == nullptr) return true;

	if (Oper->Tag == "Player")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Equipment", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
		
	}

	else if (Oper->Tag == "Equipment")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Equipment", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
	}

	else if (Oper->Tag == "Inventory")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Equipment", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
	}

	else if (Oper->Tag == "Shop")
	{
		//create ui confirm by widget
		//add to viewport
	}

	return true;

}


FSlateBrush UUI_EquipmentSlot::MakeBrush()
{
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemData.Icon, 0, 0);

	return Brush;
}

void UUI_EquipmentSlot::RefreshWidget(class AMyInvenCharacter* Cha)
{
	if (Cha == nullptr) return;
	if (Cha->IsShopOpen)
	{
		Cha->RefreshShopWidget();
	}
	else
	{
		Cha->RefreshInvWidget();
	}
}
