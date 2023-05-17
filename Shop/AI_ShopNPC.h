// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_Interaction.h"
#include "MyInven/CustomStruct.h"
#include "Blueprint/UserWidget.h"
#include "AI_ShopNPC.generated.h"

UCLASS()
class MYINVEN_API AAI_ShopNPC : public ACharacter , public IAI_Interaction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_ShopNPC();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	   
	// Redefine AI_Interface Function
	void StartInteraction(class AMyInvenCharacter* Character) override;

	void BuyItem(FSlot Item, int32 Index) override;

	void SellItem(FSlot Item) override;
	
	UPROPERTY(Replicated)
	TArray<FSlot> Inventory;

	void AddItem(FSlot Item);

	void FindStack(FSlot Item, bool& Success, int32& Index);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShopWidgetClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

	

};
