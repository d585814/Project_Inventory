// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_ItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize() override;
	
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemAmountText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDescText;

	UPROPERTY()
	FInventory SInv;

	int32 Amount=0;

	UFUNCTION()
	FText GetItemAmount();

	UFUNCTION()
	FText GetItemName();

	UFUNCTION()
	FText GetItemDesc();
	
};
