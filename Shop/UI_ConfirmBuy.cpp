// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ConfirmBuy.h"
#include "Components/Button.h"
#include "MyInven/MyInvenCharacter.h"

void UUI_ConfirmBuy::NativeConstruct()
{

}

bool UUI_ConfirmBuy::Initialize()
{	
	bool Success = Super::Initialize();
	if (!Success) return false;



	if (YesButton == nullptr) return false;
	YesButton->OnClicked.AddDynamic(this, &UUI_ConfirmBuy::YesButtonClicked);

	if (NoButton == nullptr) return false;
	NoButton->OnClicked.AddDynamic(this, &UUI_ConfirmBuy::NoButtonClicked);

	return true;
}

void UUI_ConfirmBuy::YesButtonClicked()
{
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(Item.Item.RowName, "");
	

	if (OutRow != nullptr)
	{
		if (Source == "Player")
		{
			Player->MoveItem(Item,Source,Direction,ItemIndex,SlotIndex);
		}
		else if (Source == "Inventory")
		{
			Player->MoveItem(Item, Source, Direction, ItemIndex, SlotIndex);
		}
		else if (Source == "Equipment")
		{
			Player->MoveItem(Item, Source, Direction, ItemIndex, SlotIndex);
		}
		else if (Source == "Shop")
		{
			if (Player->GetMoney() >= Item.Amount*OutRow->Value)
			{
				if (Player->AddItem(Item))
				{
					Player->SetMoney(Player->GetMoney() - (Item.Amount*OutRow->Value));
					Player->ServerBuyItem(Player->Interactable, ItemIndex, Item);
				}
			}
		}
	}
	NoButtonClicked();
}

void UUI_ConfirmBuy::NoButtonClicked()
{
	this->RemoveFromParent();
	
	// Delay and RefreshShopWidget
	FTimerHandle Handle;
	float WaitTime=0.1f; 
	GetOwningPlayer()->GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
	{
		Player->RefreshShopWidget();
		GetOwningPlayer()->GetWorld()->GetTimerManager().ClearTimer(Handle);
	}), WaitTime, false); 	
	
}