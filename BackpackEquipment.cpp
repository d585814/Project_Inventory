// Fill out your copyright notice in the Description page of Project Settings.


#include "BackpackEquipment.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MyInvenCharacter.h"

ABackpackEquipment::ABackpackEquipment()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
	Tags.Add(FName("Pickup"));

	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("Pickup"));
	GetStaticMeshComponent()->SetSimulatePhysics(true);
		
	
}

void ABackpackEquipment::OnConstruction(const FTransform& Transform)
{
	
}

void ABackpackEquipment::BeginPlay()
{
	Super::BeginPlay();	

	PlayerSlot.SetNum(12);
	SetPlayerSlot(PlayerSlot, 12);
	
}

void ABackpackEquipment::SetPlayerSlot(TArray<FSlot>& Slot, int32 Int)
{
	AMyInvenCharacter* Player = Cast<AMyInvenCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Player == nullptr) return;
	if (Player->ItemDataBase == nullptr) return;
	for (int i = 0; i < Int; i++)
	{
		FDataTableRowHandle test;
		test.DataTable = Player->ItemDataBase;
		test.RowName = "Empty";
		Slot[i].Item = test;
		Slot[i].Amount = 0;
		
	}
}

void ABackpackEquipment::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABackpackEquipment, PlayerSlot);
}

bool ABackpackEquipment::AddItem(FSlot Item)
{
	bool Success;
	int32 Amount;
	int32 Index;

	FindStack(Item, Success, Amount, Index);
	if (Success)
	{
		if (AddItemToPlayerSlot(Item, Index))
		{			
			return true;
		}
		else
		{
			FindEmptySloy(Success, Index);
			if (Success)
			{
				
				return AddItemToPlayerSlot(Item, Index);
			}
			
		}
	}

	else
	{
		FindEmptySloy(Success, Index);
		if (Success)
		{
			return AddItemToPlayerSlot(Item, Index);
		}
				
	}
	return false;
}

void ABackpackEquipment::FindStack(FSlot Slot, bool& Success, int32& Amount, int32& Index)
{
	bool LocalFound = false;
	int32 LocalAmount = 0;
	int32 LocalIndex = 0;
	int i = 0;
	for (auto MySlot : PlayerSlot)
	{
		if (MySlot.Item.RowName == Slot.Item.RowName)
		{
			LocalFound = true;
			LocalAmount = MySlot.Amount;
			LocalIndex = i;

			break;
		}
		i++;
	}
	Success = LocalFound;
	Amount = LocalAmount;
	Index = LocalIndex;
}

void ABackpackEquipment::FindEmptySloy(bool& Success, int32& Index)
{
	bool LocalFound = false;
	int32 LocalIndex = 0;
	int i = 0;
	for (auto MySlot : PlayerSlot)
	{
		if (MySlot.Item.RowName == "Empty")
		{
			LocalFound = true;
			LocalIndex = i;

			break;
		}
		i++;
	}
	Success = LocalFound;
	Index = LocalIndex;
}

bool ABackpackEquipment::RemoveAmountAtIndex(int32 Index, int32 Amount)
{
	int32 Num = PlayerSlot[Index].Amount - Amount;
	
	if (Num >= 0)
	{
		if (Num > 0)
		{
			FSlot test;
			test.Amount = Num;
			test.Item = PlayerSlot[Index].Item;

			PlayerSlot[Index] = test;
			return true;
		}
		else
		{
			MakeEmptySlot(PlayerSlot, Index);
			return true;
		}
	}
	else
	{
		return false;
	}
	
}

void ABackpackEquipment::MakeEmptySlot(TArray<FSlot>& Array, int32 Index)
{
	AMyInvenCharacter* Player = Cast<AMyInvenCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) return;
	if (Player->ItemDataBase == nullptr) return;

	Array[Index].Item.DataTable = Player->ItemDataBase;
	Array[Index].Item.RowName = "Empty";
	Array[Index].Amount = 0;
}

bool ABackpackEquipment::RemoveItem(FSlot Item)
{
	bool bValue = false;
	int32 AmountValue = 0;
	int32 IndexValue = 0;


	FindStack(Item, bValue, AmountValue, IndexValue);
	if (bValue)
	{
		if (AmountValue - Item.Amount >= 0)
		{
			if (AmountValue - Item.Amount == 0)
			{
				MakeEmptySlot(PlayerSlot, IndexValue);
				return true;
			}
			else
			{
				PlayerSlot[IndexValue].Item = Item.Item;
				PlayerSlot[IndexValue].Amount = AmountValue - Item.Amount;
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}

bool ABackpackEquipment::AddItemToPlayerSlot(FSlot Item, int32 Index)
{
	if (PlayerSlot[Index].Item.RowName == "Empty")
	{
		PlayerSlot[Index] = Item;
		return true;
	}
	else
	{
		FName LocalRowName = Item.Item.RowName;
		FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName, "");

		if (OutRow != nullptr)
		{
			if (PlayerSlot[Index].Item.RowName == Item.Item.RowName && OutRow->CanStack)
			{
				FSlot SlotValue;
				SlotValue.Item = Item.Item;
				SlotValue.Amount = PlayerSlot[Index].Amount + Item.Amount;

				PlayerSlot[Index] = SlotValue;
				return true;
			}

		}
		return false;
	}
}


void ABackpackEquipment::PickupInterection(class AMyInvenCharacter* Player)
{	
	if (Player == nullptr) return;

	if (!IsValid(Player->Backpack) )
	{		
		Player->Backpack = this;
		if (GetStaticMeshComponent() == NULL) return;
		Player->ServerAttachBackpack(this, Player->GetMesh(), GetStaticMeshComponent(), Scale);
		
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Yes Backpack!"), *Player->Backpack->GetName())

	}
}
