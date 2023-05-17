// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomStruct.generated.h"

// Structure_Slot
USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (MakeStructureDefaultValue = 0))
	int32 Amount;
};

// Structure_Inventory
USTRUCT(BlueprintType)
struct FInventory : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = "None"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = nullptr))
	class UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = nullptr))
	class UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = false))
	bool CanStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = false))
	bool CanCraft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Recepie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = 0))
	int32 OutCraftAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = 0))
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Actor;
};


// Structure_Equipment
USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeStructureDefaultValue = nullptr))
	class AActor* Actor;
};




UCLASS()
class MYINVEN_API ACustomStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomStruct();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};

