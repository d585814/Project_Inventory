// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CraftableTooltip.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI_Craftable.h"
#include "MyInven/MyController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"




void UUI_CraftableTooltip::NativeConstruct()
{	
	AMyController*cont = Cast<AMyController>(GetOwningPlayer());
	if (cont == nullptr) return;
	
	for (auto Rec : ItemData.Recepie)
	{
		FInventory* OutRow = Rec.Item.DataTable->FindRow<FInventory>(Rec.Item.RowName, "");
		if (OutRow != nullptr)
		{
			Craft = CreateWidget<UUI_Craftable>(GetOwningPlayer(), cont->CraftWidgetClass);
			if (Craft != nullptr)
			{
				
				Craft->Amount = Rec.Amount;
				Craft->ItemData = *OutRow;
				Craft->RowName = "None";
				
				/*if (HoriBox != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Fucking HoriBox"));
					HoriBox->AddChildToHorizontalBox(Craft);
				}*/

			}
		}
	}
}

bool UUI_CraftableTooltip::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (Tooltip == nullptr) return false;
	Tooltip->BackgroundDelegate.BindDynamic(this, &UUI_CraftableTooltip::MakeBrush);
		
	if (CraftAmountText == nullptr) return false;
	CraftAmountText->TextDelegate.BindDynamic(this, &UUI_CraftableTooltip::GetCraftAmount);
	
	return true;
}

FSlateBrush UUI_CraftableTooltip::MakeBrush()
{
	UTexture2D* Texture=nullptr;
	FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(Texture, 0, 0);
	if (Craft != nullptr)
	{
		Brush = UWidgetBlueprintLibrary::MakeBrushFromTexture(Craft->ItemData.Icon, 0, 0);
	}
	return Brush;
}

FText UUI_CraftableTooltip::GetCraftAmount()
{
	FText text = FText::FromString("");
	if (Craft != nullptr)
	{
		text = FText::FromString(FString::FromInt(Craft->Amount));
	}
	return text;
}