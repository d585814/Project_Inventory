// Fill out your copyright notice in the Description page of Project Settings.


#include "MyController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AMyController::AMyController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> FindItemWidgetClass(TEXT("/Game/UI/WBP_ItemData"));
	if (FindItemWidgetClass.Class != nullptr)
	{
		ItemDataWidgetClass = FindItemWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindInvPanelWidgetClass(TEXT("/Game/UI/WBP_InvPanel"));
	if (FindInvPanelWidgetClass.Class != nullptr)
	{
		InvPanelWidgetClass = FindInvPanelWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindDropBoxClass(TEXT("/Game/UI/WBP_DropBox"));
	if (FindDropBoxClass.Class != nullptr)
	{
		DropBoxWidgetClass = FindDropBoxClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindCraftClass(TEXT("/Game/UI/WBP_Craftable"));
	if (FindCraftClass.Class != nullptr)
	{
		CraftWidgetClass = FindCraftClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindEquipClass(TEXT("/Game/UI/WBP_EquipPanel"));
	if (FindEquipClass.Class != nullptr)
	{
		EquipWidgetClass = FindEquipClass.Class;
	}
	
	ConstructorHelpers::FClassFinder<UUserWidget> FindShopPanelClass(TEXT("/Game/UI/Shop/WBP_ShopPanel"));
	if (FindShopPanelClass.Class != nullptr)
	{
		ShopPanelWidgetClass = FindShopPanelClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FindConfirmBuyClass(TEXT("/Game/UI/Shop/WBP_ConfirmBuy"));
	if (FindConfirmBuyClass.Class != nullptr)
	{
		ConfirmBuyWidgetClass = FindConfirmBuyClass.Class;
	}
}