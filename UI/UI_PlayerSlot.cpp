// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerSlot.h"
#include "MyInven/MyInvenCharacter.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "UI_ItemTooltip.h"
#include "MyInven/MyController.h"
#include "MyInven/Shop/UI_ConfirmBuy.h"

void UUI_PlayerSlot::NativeConstruct()
{
	AMyInvenCharacter* Character = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Character == nullptr) return;
	Item = Character->PlayerSlot[SlotIndex];

	if(Item.Item.DataTable ==nullptr) return;
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(Item.Item.RowName, "");

	if (OutRow == nullptr) return;
	ItemData = *OutRow;

	//Set Tooltip Widget
	if (ItemTooltipClass == NULL) return;
	UUI_ItemTooltip* Tooltip = CreateWidget<UUI_ItemTooltip>(GetOwningPlayer(), ItemTooltipClass);

	if (Tooltip == nullptr) return;
	Tooltip->SInv = ItemData;
	Tooltip->Amount = Item.Amount;

	if (SlotDataP == nullptr) return;
	SlotDataP->SetToolTip(Tooltip);
}

bool UUI_PlayerSlot::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (SlotDataP == nullptr) return false;
	SlotDataP->BackgroundDelegate.BindDynamic(this, &UUI_PlayerSlot::MakeBrush);


	return true;
}

FSlateBrush UUI_PlayerSlot::MakeBrush()
{
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemData.Icon, 0, 0);

	return Brush;
}

FReply UUI_PlayerSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;
}

void UUI_PlayerSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);


	if (DragDropOperClass == NULL) return;
	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperClass));//NewObject<UItemDragDropOperation>();

	FSlot OperItem;
	OperItem.Item = Item.Item;
	OperItem.Amount = 1;

	// Set ItemDragDropOperation
	Oper->Pivot = EDragPivot::CenterCenter;
	Oper->Tag = "Player";
	Oper->DefaultDragVisual = this;
	Oper->Item = OperItem;
	Oper->Index = SlotIndex;
	

	OutOperation = Oper;
}

bool UUI_PlayerSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(InOperation);
	if (Oper==nullptr) return true;

	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Cha == nullptr) return true;

	if (Oper->Tag == "Player")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Player", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
	}

	else if (Oper->Tag == "Equipment")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Player", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
	}

	else if (Oper->Tag == "Inventory")
	{
		Cha->MoveItem(Oper->Item, Oper->Tag, "Player", Oper->Index, SlotIndex);
		RefreshWidget(Cha);
	}

	else if (Oper->Tag == "Shop")
	{
		//create ui confirm by widget
		AMyController* cont = Cast<AMyController>(GetOwningPlayer());
		if (cont == nullptr) return true;
		if(cont->ConfirmBuyWidgetClass ==nullptr) return true;

		UUI_ConfirmBuy* ConfirmWidget = CreateWidget<UUI_ConfirmBuy>(GetOwningPlayer(), cont->ConfirmBuyWidgetClass);
		if (ConfirmWidget == nullptr) return true;

		// Set ConfirmWidget
		ConfirmWidget->Player = Cha;
		ConfirmWidget->ItemIndex = Oper->Index;
		ConfirmWidget->SlotIndex = SlotIndex;
		ConfirmWidget->Direction = "Player";
		ConfirmWidget->Source = Oper->Tag;
		ConfirmWidget->Item = Oper->Item;

		//add to viewport
		ConfirmWidget->AddToViewport();
	}

	return true;

}

void UUI_PlayerSlot::RefreshWidget(class AMyInvenCharacter* Cha)
{
	if (Cha->IsShopOpen)
	{
		Cha->RefreshShopWidget();
	}
	else
	{
		Cha->RefreshInvWidget();
	}
}