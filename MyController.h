// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyController.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API AMyController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyController();

	UPROPERTY()
	TSubclassOf<class UUserWidget> ItemDataWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> InvPanelWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> DropBoxWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> CraftWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> EquipWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ShopPanelWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ConfirmBuyWidgetClass;

	
};
