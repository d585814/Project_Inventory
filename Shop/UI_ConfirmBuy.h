// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInven/CustomStruct.h"
#include "UI_ConfirmBuy.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_ConfirmBuy : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* YesButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NoButton;

	UFUNCTION()
	void YesButtonClicked();

	UFUNCTION()
	void NoButtonClicked();

	class AMyInvenCharacter* Player;

	FSlot Item;

	int32 ItemIndex;

	int32 SlotIndex;

	FString Direction;

	FString Source;


};
