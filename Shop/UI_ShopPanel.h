// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ShopPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_ShopPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	AActor* NPC;

	int32 SlotPerRow=4;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ConfirmWidgetClass;

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
};
