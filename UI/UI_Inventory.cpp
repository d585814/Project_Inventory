// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "UI_InvPanel.h"
#include "MyInven/MyController.h"
#include "UI_DropBox.h"
#include "UI_Craftable.h"
#include "UI_EquipPanel.h"
#include "MyInven/MyInvenCharacter.h"


void UUI_Inventory::NativeConstruct()
{
	AMyInvenCharacter* Player = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	AMyController*cont = Cast<AMyController>(GetOwningPlayer());
	if (cont != nullptr && Player !=nullptr)
	{
		if (cont->InvPanelWidgetClass == NULL) return;
		if (cont->DropBoxWidgetClass == NULL) return;
		if (cont->EquipWidgetClass == NULL) return; 

		UUI_InvPanel* InvPanelW = CreateWidget<UUI_InvPanel>(GetOwningPlayer(), cont->InvPanelWidgetClass);
		UUI_DropBox* Drop = CreateWidget<UUI_DropBox>(GetOwningPlayer(), cont->DropBoxWidgetClass);
		UUI_EquipPanel* Equip = CreateWidget<UUI_EquipPanel>(GetOwningPlayer(), cont->EquipWidgetClass);

		if (InvPanelW == nullptr) return;
		if (Drop == nullptr) return;
		if (Equip == nullptr) return;
		
		if (InvPanel == nullptr) return;
		if (DropBox == nullptr) return;
		if (EquipmentPanel == nullptr) return;

		InvPanel->AddChild(InvPanelW);
		DropBox->AddChild(Drop);
		EquipmentPanel ->AddChild(Equip);

		if (Player->ItemDataBase == nullptr) return;
		for (auto Name : Player->ItemDataBase->GetRowNames())
		{			
			FInventory* OutRow = Player->ItemDataBase->FindRow<FInventory>(Name, "");
			if (OutRow != nullptr)
			{
				if (OutRow->CanCraft)
				{
					UUI_Craftable* Craft = CreateWidget<UUI_Craftable>(GetOwningPlayer(), cont->CraftWidgetClass);
					if (Craft != nullptr)
					{
						Craft->Amount = OutRow->OutCraftAmount;
						Craft->ItemData = *OutRow;
						Craft->RowName = Name;

						if (CraftableList == nullptr) return;
						CraftableList->AddChild(Craft);
					}
				}
			}
		}

		
	}
}