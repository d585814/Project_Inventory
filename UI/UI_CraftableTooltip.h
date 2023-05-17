// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_CraftableTooltip.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_CraftableTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void NativeConstruct() override;

	bool Initialize() override;
	

	UPROPERTY(meta = (BindWidget))
	class UBorder* Tooltip;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CraftAmountText;

	FInventory ItemData;

	UFUNCTION()
	FSlateBrush MakeBrush();

	UFUNCTION()
	FText GetCraftAmount();

private:
	UPROPERTY()
	class UUI_Craftable* Craft;

	
};
