// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Shop.h"
#include "Components/Border.h"
#include "MyInven/MyController.h"
#include "MyInven/MyInvenCharacter.h"
#include "MyInven/UI/UI_InvPanel.h"
#include "MyInven/UI/UI_EquipPanel.h"
#include "UI_ShopPanel.h"


void UUI_Shop::NativeConstruct()
{
	AMyController*cont = Cast<AMyController>(GetOwningPlayer());
	if (cont == nullptr) return;

	AMyInvenCharacter* Player = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Player == nullptr) return;

	if (cont->InvPanelWidgetClass == NULL) return;
	if (cont->EquipWidgetClass == NULL) return;
	if (cont->ShopPanelWidgetClass == NULL) return;

	if (InvPanel == nullptr) return;
	if (EquipPanel == nullptr) return;
	if (ShopPanel == nullptr) return;


	UUI_InvPanel* InvWidget = CreateWidget<UUI_InvPanel>(GetOwningPlayer(), cont->InvPanelWidgetClass);
	if (InvWidget == nullptr) return;
	InvPanel->AddChild(InvWidget);

	UUI_EquipPanel* EquipWidget= CreateWidget<UUI_EquipPanel>(GetOwningPlayer(), cont->EquipWidgetClass);
	if (EquipWidget == nullptr) return;
	EquipPanel->AddChild(EquipWidget);

	UUI_ShopPanel* ShopWidget = CreateWidget<UUI_ShopPanel>(GetOwningPlayer(), cont->ShopPanelWidgetClass);
	if (ShopWidget == nullptr) return;
	ShopWidget->NPC = Player->Interactable;
	ShopPanel->AddChild(ShopWidget);


}