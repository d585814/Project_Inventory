// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_PlayerSlot.generated.h"


/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_PlayerSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	bool Initialize() override;

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;

	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotDataP;

	UPROPERTY()
	FInventory ItemData;

	UPROPERTY()
	FSlot Item;

	UFUNCTION(BlueprintPure)
	FSlateBrush MakeBrush();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDragDropOperation> DragDropOperClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemTooltipClass;

	void RefreshWidget(class AMyInvenCharacter* Cha);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	
};
