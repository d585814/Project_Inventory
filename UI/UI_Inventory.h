// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_Inventory.generated.h"


/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	

	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBorder* InvPanel;

	UPROPERTY(meta = (BindWidget))
	class UBorder* EquipmentPanel;

	UPROPERTY(meta = (BindWidget))
	class UBorder* DropBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CraftableList;
		

	
};
