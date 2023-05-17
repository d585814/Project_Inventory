// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ShopPanel.h"
#include "Components/UniformGridPanel.h"
#include "AI_ShopNPC.h"
#include "MyInven/UI/UI_ItemData.h"
#include "MyInven/MyController.h"
#include "MyInven/MyInvenCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/UniformGridSlot.h"
#include "MyInven/UI/ItemDragDropOperation.h"
#include "UI_ConfirmBuy.h"


void UUI_ShopPanel::NativeConstruct()
{
	
	AAI_ShopNPC* shopNPC = Cast<AAI_ShopNPC>(NPC);
	if (shopNPC == nullptr) return;
		
	AMyController* cont = Cast<AMyController>(GetOwningPlayer());
	if (cont == nullptr) return;

	int32 i = 0;
	for (auto shopInv : shopNPC->Inventory)
	{
		UUI_ItemData* ItemWidget = CreateWidget<UUI_ItemData>(GetOwningPlayer(),cont->ItemDataWidgetClass);
		if (ItemWidget != nullptr)
		{
			ItemWidget->Item = shopInv;
			ItemWidget->Index = i;
			ItemWidget->Source = "Shop";
			
			float fi = i;
			float fSlotPerRow = SlotPerRow;
			int32 test = UKismetMathLibrary::FTrunc(fi / fSlotPerRow);
			float fRemiander;
			UKismetMathLibrary::FMod(fi, fSlotPerRow, fRemiander);
			int32 Remainder = UKismetMathLibrary::FTrunc(fRemiander);

			if (GridPanel == nullptr) return;
			UUniformGridSlot * Grid = GridPanel->AddChildToUniformGrid(ItemWidget, test, Remainder);
			if (Grid == nullptr) return;
			Grid->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			Grid->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
		i++;
	}

}

bool UUI_ShopPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UE_LOG(LogTemp, Warning, TEXT("Shop Drop"));

	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(InOperation);
	if (Oper==nullptr) return true;

	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Cha == nullptr) return true;

	AMyController* cont = Cast<AMyController>(GetOwningPlayer());
	if (cont == nullptr) return true;

	if(ConfirmWidgetClass==NULL) return true;
	UUI_ConfirmBuy* ConfirmWidget = CreateWidget<UUI_ConfirmBuy>(GetOwningPlayer(), ConfirmWidgetClass);
	if (ConfirmWidget != nullptr)
	{
		//Set ConfirmWidget
		ConfirmWidget->Player = Cha;
		ConfirmWidget->ItemIndex = Oper->Index;
		ConfirmWidget->SlotIndex = 0;
		ConfirmWidget->Direction = "Shop";
		ConfirmWidget->Source = Oper->Tag;
		ConfirmWidget->Item = Oper->Item;

		// ConfirmWidget Add To Viewport
		ConfirmWidget->AddToViewport();
	}



	return true;
}