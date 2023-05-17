// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemTooltip.h"
#include "Components/TextBlock.h"

bool UUI_ItemTooltip::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (ItemAmountText == nullptr) return false;
	ItemAmountText->TextDelegate.BindDynamic(this, &UUI_ItemTooltip::GetItemAmount);

	if (ItemNameText == nullptr) return false;
	ItemNameText->TextDelegate.BindDynamic(this, &UUI_ItemTooltip::GetItemName);

	if (ItemDescText == nullptr) return false;
	ItemDescText->TextDelegate.BindDynamic(this, &UUI_ItemTooltip::GetItemDesc);


	return true;
}

FText UUI_ItemTooltip::GetItemAmount()
{
	FText text = FText::FromString(" ");
	if (Amount > 0)
	{		
		return FText::FromString(FString::FromInt(Amount));
	}

	return text;
}

FText UUI_ItemTooltip::GetItemName()
{
	return FText::FromString(SInv.Name);
}

FText UUI_ItemTooltip::GetItemDesc()
{
	return FText::FromString(SInv.Desc);
}