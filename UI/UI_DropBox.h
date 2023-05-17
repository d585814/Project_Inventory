// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_DropBox.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_DropBox : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
