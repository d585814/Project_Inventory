// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_DropBox.h"
#include "ItemDragDropOperation.h"
#include "MyInven/BackpackEquipment.h"
#include "MyInven/MyInvenCharacter.h"
#include "Components/CapsuleComponent.h"

bool UUI_DropBox::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	//if (!Success) return false;
	
	UItemDragDropOperation* Oper = Cast<UItemDragDropOperation>(InOperation);
	if(Oper==nullptr) return true;

	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Cha == nullptr) return true;

	

	FTransform Trans;
	Trans.SetLocation(Cha->GetCapsuleComponent()->GetComponentLocation() + (Cha->GetCapsuleComponent()->GetForwardVector() * 100));

	if (Oper->Tag == "Inventory")
	{
		
		if (Cha->Backpack->RemoveAmountAtIndex(Oper->Index, Oper->Item.Amount))
		{			
			Cha->ServerSpawnItem(Trans, Oper->Item);
			Cha->RefreshInvWidget();			
		}
	}
	else if (Oper->Tag == "Player")
	{
		if (Cha->RemoveAmountAtIndex(Oper->Index, Oper->Item.Amount, Cha->PlayerSlot))
		{
			Cha->ServerSpawnItem(Trans, Oper->Item);
			Cha->RefreshInvWidget();
			
		}
	}
	else if (Oper->Tag == "Equipment")
	{
		if (Cha->RemoveAmountAtIndex(Oper->Index, Oper->Item.Amount, Cha->Equipment))
		{
			Cha->ServerSpawnItem(Trans, Oper->Item);
			Cha->RefreshInvWidget();
			
		}
	}
	

	return true;
}