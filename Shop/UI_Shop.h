// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Shop.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_Shop : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBorder* EquipPanel;

	UPROPERTY(meta = (BindWidget))
	class UBorder* InvPanel;

	UPROPERTY(meta = (BindWidget))
	class UBorder* ShopPanel;
	
};
