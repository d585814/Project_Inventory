// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomStruct.h"
#include "PreviewActor.generated.h"

UCLASS()
class MYINVEN_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UStaticMeshComponent* HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UStaticMeshComponent* TopMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UStaticMeshComponent* BackWeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class USceneCaptureComponent2D* SceneCapture;

	UPROPERTY()
	class UMaterial* MatRender2D;
	
	UFUNCTION()
	void SetMeshLocation(TArray<FEquipment> Equipment);

	UFUNCTION()
	void SetEquipmentMesh(FDataTableRowHandle Item, int32 Index);

	UFUNCTION()
	void RemoveEquipmentMesh(int32 Index);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	



};
