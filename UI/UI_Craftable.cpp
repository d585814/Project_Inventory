// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Craftable.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "MyInven/MyInvenCharacter.h"
#include "MyInven/BackpackEquipment.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI_CraftableTooltip.h"


void UUI_Craftable::NativeConstruct()
{		
	
	Character = Cast<AMyInvenCharacter>(GetOwningPlayerPawn());
	if (Character == nullptr) return;

	if (CraftTooltipClass == NULL) return;
	UUI_CraftableTooltip* Tooltip = CreateWidget<UUI_CraftableTooltip>(GetOwningPlayer(), CraftTooltipClass);

	if (Tooltip == nullptr) return;
	Tooltip->ItemData = ItemData;

	if (CraftBorder == nullptr) return;
	CraftBorder->SetToolTip(Tooltip);
	
}

void UUI_Craftable::TooltipTest()
{
	
}

bool UUI_Craftable::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (CraftBorder == nullptr) return false;
	CraftBorder->BackgroundDelegate.BindDynamic(this, &UUI_Craftable::MakeBrush);

	if (Text == nullptr) return false;
	Text->TextDelegate.BindDynamic(this, &UUI_Craftable::GetCraftText);

	return true;
}

FReply UUI_Craftable::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply ReplyResult;
	bool LocalFail=false;
	bool ResultBool;
	int32 ResultIndex;
	int32 ResultAmount;
	TArray<int32> LocalAAmount;
	TArray<int32> LocalAIndex;
	
	if (Character == nullptr) return ReplyResult.NativeReply;

	for (auto rec : ItemData.Recepie)
	{
		if (IsValid(Character->Backpack))
		{
			Character->Backpack->FindStack(rec, ResultBool, ResultAmount, ResultIndex);
			if (ResultBool && ResultAmount >= rec.Amount)
			{
				LocalAAmount.Add(rec.Amount);
				LocalAIndex.Add(ResultIndex);
			}
			else
			{
				LocalFail = true;
				break;
			}
		}
	}

	if (LocalFail)
	{
		return ReplyResult.NativeReply;
	}
	else
	{
		if (IsValid(Character->Backpack))
		{
			FSlot NewSlot;
			NewSlot.Item.DataTable = Character->ItemDataBase;
			NewSlot.Item.RowName = RowName;
			NewSlot.Amount = ItemData.OutCraftAmount;

			if (Character->Backpack->AddItem(NewSlot))
			{
				int32 i = 0;
				for (auto Array : LocalAIndex)
				{
					Character->Backpack->RemoveAmountAtIndex(Array, LocalAAmount[i]);
				}
				Character->RefreshInvWidget();
				
			}
			
		}
		return ReplyResult.NativeReply;
	}
}

FSlateBrush UUI_Craftable::MakeBrush()
{
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemData.Icon, 0, 0);

	return Brush;
}

FText UUI_Craftable::GetCraftText()
{
	return FText::FromString(FString::FromInt(Amount));
}
