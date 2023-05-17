// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CustomStruct.h"
#include "PickupInterection.h"
#include "BackpackEquipment.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API ABackpackEquipment : public AStaticMeshActor, public IPickupInterection
{
	GENERATED_BODY()
	
public:
	ABackpackEquipment(); //virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	TArray<FSlot> PlayerSlot;

	UFUNCTION()
	bool AddItem(FSlot Item);

	UFUNCTION()
	void FindStack(FSlot Slot, bool& Found, int32& Amount, int32& Index);

	UFUNCTION()
	void FindEmptySloy(bool& Success, int32& Index);

	UFUNCTION()
	bool RemoveAmountAtIndex(int32 Index, int32 Amount);

	UFUNCTION()
	bool RemoveItem(FSlot Item);

	UFUNCTION()
	bool AddItemToPlayerSlot(FSlot Item, int32 Index);

	UFUNCTION()
	void PickupInterection(class AMyInvenCharacter* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale;

private:
	UFUNCTION()
	void MakeEmptySlot(TArray<FSlot>& Array, int32 Index);

	UFUNCTION()
	void SetPlayerSlot(TArray<FSlot>& Slot, int32 Int);
	
		
};
