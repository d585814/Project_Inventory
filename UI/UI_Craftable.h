// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_Craftable.generated.h"


/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_Craftable : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UBorder* CraftBorder;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;

	UFUNCTION()
	FSlateBrush MakeBrush();

	UFUNCTION()
	FText GetCraftText();

	int32 Amount;

	UPROPERTY()
	FInventory ItemData;
		
	FName RowName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CraftTooltipClass;

	UFUNCTION()
	void TooltipTest();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	
	class AMyInvenCharacter* Character;

	
};
