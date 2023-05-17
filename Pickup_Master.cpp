// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Master.h"
#include "Net/UnrealNetwork.h"
#include "MyInvenCharacter.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

APickup_Master::APickup_Master()
{
	bReplicates = true;
	SetReplicateMovement(true);

	Tags.Add(FName("Pickup")); 

	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("Pickup"));
	//GetStaticMeshComponent()->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Inventory/ItemData"));
	if (DataTable.Succeeded())
	{		
		ItemDataBase = DataTable.Object;
	}
	
	if (ItemDataBase == nullptr) return;
	Item.Item.DataTable = ItemDataBase;
	Item.Item.RowName = "Empty";
		
}

void APickup_Master::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ChangeMesh();
}

void APickup_Master::BeginPlay()
{
	Super::BeginPlay();
		
}

void APickup_Master::ChangeMesh()
{
	FName LocalRowName = Item.Item.RowName;
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName, "");
	if (OutRow != nullptr)
	{		
		GetStaticMeshComponent()->SetStaticMesh(OutRow->Mesh);
	}
}

void APickup_Master::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickup_Master, Item);
}

void APickup_Master::PickupInterection(class AMyInvenCharacter* Player)
{
	if (Player == nullptr) return;
	
	if (Player->AddItem(Item))
	{		
		Player->ServerDestroy(this);
	}
}