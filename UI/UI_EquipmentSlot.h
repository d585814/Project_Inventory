// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_EquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UBorder* EquipBorder;
		
	UPROPERTY()
	FSlot Item;

	UPROPERTY()
	FInventory ItemData;

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;

	UFUNCTION()
	FSlateBrush MakeBrush();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemTooltipClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDragDropOperation> DragDropOperClass;

	void RefreshWidget(class AMyInvenCharacter* Cha);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
