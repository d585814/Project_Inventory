// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStruct.h"
#include "UObject/ConstructorHelpers.h"




// Sets default values
ACustomStruct::ACustomStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

}

// Called when the game starts or when spawned
void ACustomStruct::BeginPlay()
{
	Super::BeginPlay();

	
}

