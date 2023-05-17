// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CustomStruct.h"
#include "PickupInterection.h"
#include "Pickup_Master.generated.h"


/**
 * 
 */
UCLASS()
class MYINVEN_API APickup_Master : public AStaticMeshActor , public IPickupInterection
{
	GENERATED_BODY()

public:
	APickup_Master();

	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void PickupInterection(class AMyInvenCharacter* Player) override;
		
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FSlot Item;

	void ChangeMesh();
protected:

	UPROPERTY()
	class UDataTable* ItemDataBase;

private:
	  
};
