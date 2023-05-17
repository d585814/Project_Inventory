// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MyInven/CustomStruct.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int32 Index;

	UPROPERTY()
	FSlot Item;
	
};
