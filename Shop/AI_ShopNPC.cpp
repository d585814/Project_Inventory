// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_ShopNPC.h"
#include "MyInven/MyInvenCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAI_ShopNPC::AAI_ShopNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	Tags.Add(FName("NPC"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pickup"));
}

// Called when the game starts or when spawned
void AAI_ShopNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAI_ShopNPC::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAI_ShopNPC, Inventory);	
}

void AAI_ShopNPC::StartInteraction(class AMyInvenCharacter* Character)
{
	Character->Interactable = this;
	if (ShopWidgetClass == NULL) return;
	Character->MakeWidget(ShopWidgetClass);

	if (Character->IsShopOpen)
	{
		Character->IsShopOpen = false;
	}
	else
	{
		Character->IsShopOpen = true;
	}
}

void AAI_ShopNPC::BuyItem(FSlot Item, int32 Index)
{
	if (Inventory[Index].Amount > Item.Amount)
	{
		FSlot NewSlot;
		NewSlot.Item = Inventory[Index].Item;
		NewSlot.Amount = Inventory[Index].Amount - Item.Amount;

		Inventory[Index] = NewSlot;
	}
	else
	{		
		Inventory.RemoveAt(Index);
	}
}

void AAI_ShopNPC::SellItem(FSlot Item)
{
	AddItem(Item);
}

void AAI_ShopNPC::FindStack(FSlot Item, bool& Success, int32& Index)
{
	bool LocalFound = false;
	int32 LocalIndex = 0;

	for (auto Inv : Inventory)
	{
		if (Inv.Item.RowName == Item.Item.RowName)
		{
			LocalFound = true;
			LocalIndex = 0;
		}
	}

	if (LocalFound)
	{
		Success = LocalFound;
		Index = LocalIndex;
	}
	else
	{
		Success = false;
		Index = 0;
	}
}

void AAI_ShopNPC::AddItem(FSlot Item)
{
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(Item.Item.RowName, "");
	bool ResultSuccess;
	int32 ResultIndex;

	if (OutRow != nullptr)
	{
		if (OutRow->CanStack)
		{
			FindStack(Item, ResultSuccess, ResultIndex);
			if (ResultSuccess)
			{
				FSlot NewSlot;
				NewSlot.Item = Inventory[ResultIndex].Item;
				NewSlot.Amount = Inventory[ResultIndex].Amount + Item.Amount;

				Inventory[ResultIndex] = NewSlot;
			}
			else
			{
				Inventory.Add(Item);
			}
		}
		else
		{
			Inventory.Add(Item);
		}
	}
}