// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_ItemData.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_ItemData : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	bool Initialize() override;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UBorder* ItemDataP;

	UPROPERTY()
	FSlot Item;
	
	UPROPERTY()
	FInventory ItemData;

	UPROPERTY()
	int32 Index;

	UPROPERTY()
	FString Source;

	UFUNCTION()
	void CreatedSetting(FSlot NewItem, int32 NewIndex,  FString NewSource);

	UFUNCTION(BlueprintPure)
	FSlateBrush MakeBrush();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDragDropOperation> DragDropOperClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemTooltipClass;

	

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

private:
	
	
};
