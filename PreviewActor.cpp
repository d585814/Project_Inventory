// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyInvenCharacter.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APreviewActor::APreviewActor()
{


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Game/UI/Preview/K_Reder2D_Mat"));
	if (MatFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find!"));
		MatRender2D = MatFinder.Object;
	}
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(RootComponent);

	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);

	BackWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWeaponMesh"));
	BackWeaponMesh->SetupAttachment(RootComponent);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void APreviewActor::BeginPlay()
{
	Super::BeginPlay();
	AMyInvenCharacter* Cha = Cast<AMyInvenCharacter>(GetOwner());
	if (Cha != nullptr)
	{
		SceneCapture->TextureTarget= UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024);

		
		if (MatRender2D == nullptr) return;
		//UMaterial* Reder2DMat = Cast<UMaterial>(MatRender2DClass);
		UE_LOG(LogTemp, Warning, TEXT("Preview Spawn!222"));
		//if (Reder2DMat == nullptr) return;
		Cha-> RenderTargetMat =UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MatRender2D); //FName(UKismetStringLibrary::Concat_StrStr(Cha->GetName(),FString::FromInt(UKismetMathLibrary::RandomInteger(100000))))
		Cha->RenderTargetMat->SetTextureParameterValue("RenderTexture", SceneCapture->TextureTarget);

		
	}
	
}

void APreviewActor::SetMeshLocation(TArray<FEquipment> Equipment)
{
	UStaticMeshComponent* LocalMesh;
	FName LocalSocketName;
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	int32 i = 0;
	for (auto Equip : Equipment)
	{
		switch (i)
		{
		case 0:
			LocalSocketName = "Head";
			LocalMesh = HeadMesh;
			LocalMesh->AttachToComponent(Mesh, TransformRules, LocalSocketName);
			break;

		case 1:
			LocalSocketName = "Top";
			LocalMesh = TopMesh;
			LocalMesh->AttachToComponent(Mesh, TransformRules, LocalSocketName);
			break;

		case 2:
			LocalSocketName = "Weapon";
			LocalMesh = WeaponMesh;
			LocalMesh->AttachToComponent(Mesh, TransformRules, LocalSocketName);
			break;

		case 3:
			LocalSocketName = "Shield";
			LocalMesh = ShieldMesh;
			LocalMesh->AttachToComponent(Mesh, TransformRules, LocalSocketName);
			break;

		case 4:
			LocalSocketName = "BackEquip";
			LocalMesh = BackWeaponMesh;
			LocalMesh->AttachToComponent(Mesh, TransformRules, LocalSocketName);
			break;


		default:
			break;
		}
		i++;
	}
}

void APreviewActor::SetEquipmentMesh(FDataTableRowHandle Item, int32 Index)
{
	UStaticMesh* LocalMesh = nullptr;
	FInventory* OutRow = Item.DataTable->FindRow<FInventory>(Item.RowName, "");

	if (OutRow != nullptr)
	{
		LocalMesh = OutRow->Mesh;
		if(LocalMesh == nullptr)return;
		switch (Index)
		{
		case 0:
			HeadMesh->SetStaticMesh(LocalMesh);
			break;

		case 1:
			TopMesh->SetStaticMesh(LocalMesh);
			break;

		case 2:
			WeaponMesh->SetStaticMesh(LocalMesh);
			break;

		case 3:
			ShieldMesh->SetStaticMesh(LocalMesh);
			break;

		case 4:
			BackWeaponMesh->SetStaticMesh(LocalMesh);
			break;


		default:
			break;
		}
	}
}

void APreviewActor::RemoveEquipmentMesh(int32 Index)
{
	UStaticMesh* Nothing = nullptr;
	switch (Index)
	{
	case 0:
		HeadMesh->SetStaticMesh(Nothing);
		break;

	case 1:
		TopMesh->SetStaticMesh(Nothing);
		break;

	case 2:
		WeaponMesh->SetStaticMesh(Nothing);
		break;

	case 3:
		ShieldMesh->SetStaticMesh(Nothing);
		break;

	case 4:
		BackWeaponMesh->SetStaticMesh(Nothing);
		break;


	default:
		break;
	}
}

