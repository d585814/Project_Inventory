// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InvPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "MyInven/MyInvenCharacter.h"
#include "MyInven/BackpackEquipment.h"
#include "UI_ItemData.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/UniformGridSlot.h"
#include "MyInven/MyController.h"
#include "MyInven/Shop/UI_ConfirmBuy.h"
#include "Kismet/KismetMathLibrary.h"
#include "ItemDragDropOperation.h"




void UUI_InvPanel::NativeConstruct()
{	
	AMyController*cont = Cast<AMyController>(GetOwningPlayer());
	if (cont == nullptr) return;
		
	if (cont->ItemDataWidgetClass == nullptr) return;
	
	AMyInvenCharacter* Character = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Character == nullptr) return;

	if (IsValid(Character->Backpack))
	{		
		int32 i = 0;
		for (auto slot : Character->Backpack->PlayerSlot)
		{
			UUI_ItemData* Data = CreateWidget<UUI_ItemData>(GetOwningPlayer(), cont->ItemDataWidgetClass);
			if (Data == nullptr) return;
			Data->CreatedSetting(slot, i, "Inventory");
			
			
			float fi = i;
			float fSlotPerRow = SlotPerRow;
			int32 test = UKismetMathLibrary::FTrunc(fi / fSlotPerRow);
			float fRemiander;
			UKismetMathLibrary::FMod(fi, fSlotPerRow, fRemiander);
			int32 Remainder = UKismetMathLibrary::FTrunc(fRemiander);

			if (GridPanel == nullptr) return;
			UUniformGridSlot * Grid = GridPanel->AddChildToUniformGrid(Data, test, Remainder);
			if (Grid == nullptr) return;
			Grid->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			Grid->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			
			i++;
		}
	}
}

bool UUI_InvPanel::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (BackpackText == nullptr) return false;
	BackpackText->TextDelegate.BindDynamic(this, &UUI_InvPanel::IsGetBackpack);


	return true;
}

bool UUI_InvPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	

	UItemDragDropOperation* ItemOper = Cast<UItemDragDropOperation>(InOperation);
	if (ItemOper != nullptr)
	{
		AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
		if (Cha != nullptr)
		{
			if (ItemOper->Tag == "Player")
			{
				Cha->MoveItem(ItemOper->Item, ItemOper->Tag, "Inventory", ItemOper->Index, 0);
				RefreshWidget(Cha);
			}

			else if (ItemOper->Tag == "Equipment")
			{
				Cha->MoveItem(ItemOper->Item, ItemOper->Tag, "Inventory", ItemOper->Index, 0);
				RefreshWidget(Cha);
				
			}

			else if (ItemOper->Tag == "Inventory")
			{
				Cha->MoveItem(ItemOper->Item, ItemOper->Tag, "Inventory", ItemOper->Index, 0);
				RefreshWidget(Cha);
			}

			else if (ItemOper->Tag == "Shop")
			{
				//create ui confirm by widget
				AMyController* cont = Cast<AMyController>(GetOwningPlayer());
				if (cont == nullptr) return true;
				if (cont->ConfirmBuyWidgetClass == nullptr) return true;

				UUI_ConfirmBuy* ConfirmWidget = CreateWidget<UUI_ConfirmBuy>(GetOwningPlayer(), cont->ConfirmBuyWidgetClass);
				if (ConfirmWidget == nullptr) return true;

				// Set ConfirmWidget
				ConfirmWidget->Player = Cha;
				ConfirmWidget->ItemIndex = ItemOper->Index;
				ConfirmWidget->SlotIndex = 0;
				ConfirmWidget->Direction = "Inventory";
				ConfirmWidget->Source = ItemOper->Tag;
				ConfirmWidget->Item = ItemOper->Item;

				//add to viewport
				ConfirmWidget->AddToViewport();
			}
		}
	}
	
	return true;
}


FText UUI_InvPanel::IsGetBackpack()
{
	FText Text = FText::FromString("No Backpack");
	AMyInvenCharacter* Character = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Character == nullptr) return Text;
	if (IsValid(Character->Backpack) )
	{
		Text = FText::FromString("Backpack Slot");
		return Text;
	}
	return Text;
}

void UUI_InvPanel::RefreshWidget(class AMyInvenCharacter* Cha)
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