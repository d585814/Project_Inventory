// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyInvenCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Pickup_Master.h"
#include "Kismet/KismetArrayLibrary.h"
#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "BackpackEquipment.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Shop/AI_Interaction.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PreviewActor.h"


//////////////////////////////////////////////////////////////////////////
// AMyInvenCharacter

AMyInvenCharacter::AMyInvenCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	//SetSlot
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Inventory/ItemData"));
	if (DataTable.Succeeded())
	{		
		ItemDataBase = DataTable.Object;
	}

	// set Money
	Money = 1000;
	

	
	//----------------
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMyInvenCharacter::OnConstruction(const FTransform& Transform)
{
	
}

void AMyInvenCharacter::BeginPlay()
{
	Super::BeginPlay();	
	   	
	PlayerSlot.SetNum(4);
	Equipment.SetNum(6);
	EquipVisuals.SetNum(6);
	SetPlayerSlot(PlayerSlot, 4);
	SetPlayerSlot(Equipment, 6);

	if (IsLocallyControlled())
	{		
		SpawnPreviewActor();
	}
}

void AMyInvenCharacter::SetPlayerSlot(TArray<FSlot>& Slot, int32 Int)
{	
	if (ItemDataBase == nullptr) return;
	for (int i = 0; i < Int; i++)  
	{
		FDataTableRowHandle test;
		test.DataTable = ItemDataBase;
		test.RowName = "Empty";
		Slot[i].Item = test;		
		Slot[i].Amount = 0;
	}
}



//////////////////////////////////////////////////////////////////////////
// Input

void AMyInvenCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps);
	DOREPLIFETIME(AMyInvenCharacter, PlayerSlot);
	DOREPLIFETIME(AMyInvenCharacter, Equipment);
	DOREPLIFETIME(AMyInvenCharacter, EquipVisuals);
	DOREPLIFETIME(AMyInvenCharacter, Interactable);
	DOREPLIFETIME(AMyInvenCharacter, Backpack);

}
void AMyInvenCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyInvenCharacter::Interact);
	PlayerInputComponent->BindAction("ThrowBackpack", IE_Pressed, this, &AMyInvenCharacter::ThrowBackpack);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyInvenCharacter::ToggleInventory);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyInvenCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyInvenCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyInvenCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyInvenCharacter::LookUpAtRate);
}

void AMyInvenCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyInvenCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyInvenCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyInvenCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AMyInvenCharacter::Interact()
{
	TArray<AActor*> OverlapActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlapActors);
	
	for (auto Actors : OverlapActors)
	{
		//APickup_Master* Pickup = Cast<APickup_Master>(Actors);
		
		if (Actors->ActorHasTag("Pickup"))
		{			
			IPickupInterection* Interface = Cast<IPickupInterection>(Actors);
			if (Interface == nullptr) return;	
			Interface->PickupInterection(this);	
			if (IsValid(UI))
			{
				RefreshInvWidget();
			}
		}
				
		else 
		{
			if (Actors->ActorHasTag("NPC"))
			{				
				IAI_Interaction* Interface = Cast<IAI_Interaction>(Actors);
				if (Interface == nullptr) return;				
				Interface->StartInteraction(this);
			}
		}

	}
}


bool AMyInvenCharacter::AddItem(FSlot Item)
{
	bool Success;
	int32 Amount;
	int32 Index;

	if (IsValid(Backpack))
	{
		if (Backpack->AddItem(Item))
		{
			return true;
		}
		else
		{
			FindStack(Item, Success, Amount, Index);
			if (Success)
			{
				if (AddItemToPlayerSlot(Item, Index))
				{
					return true;
				}
				else
				{
					FindStack(Item, Success, Amount, Index);
					if (Success)
					{
						if (AddItemToPlayerSlot(Item, Index))
						{
							return true;
						}						
					}
				}
			}
			
		}
	}
	else
	{		
		FindStack(Item, Success, Amount, Index);
		if (Success)
		{			
			if (AddItemToPlayerSlot(Item, Index))
			{				
				return true;
			}	
			else
			{
				FindEmptySloy(Success, Index);
				if (Success)
				{
					return AddItemToPlayerSlot(Item, Index);
				}
			}
		}
		else
		{
			FindEmptySloy(Success, Index);
			if (Success)
			{
				return AddItemToPlayerSlot(Item, Index);
			}
		}
	}
	return false;
}



void AMyInvenCharacter::FindStack(FSlot Slot , bool& Success , int32& Amount, int32& Index)
{
	bool LocalFound= false;
	int32 LocalAmount=0;
	int32 LocalIndex=0;
	int i = 0;
	for (auto MySlot : PlayerSlot)
	{		
		if (MySlot.Item.RowName == Slot.Item.RowName)
		{			
			LocalFound = true;
			LocalAmount = MySlot.Amount;
			LocalIndex = i;

			break;
		}
		i++;
	}
	Success = LocalFound;
	Amount = LocalAmount;
	Index = LocalIndex;
}

void AMyInvenCharacter::FindEmptySloy(bool& Success, int32& Index )
{
	bool LocalFound= false;
	int32 LocalIndex=0;
	int i = 0;
	for (auto MySlot : PlayerSlot)
	{
		if (MySlot.Item.RowName == "Empty")
		{
			LocalFound = true;
			LocalIndex = i;

			break;
		}
		i++;
	}
	Success = LocalFound;
	Index = LocalIndex;
}

bool AMyInvenCharacter::RemoveAmountAtIndex(int32 Index ,int32 Amount , TArray<FSlot>& Array)
{
	int32 Num = Array[Index].Amount - Amount;	

	if (Num >= 0)
	{
		if (Num > 0)
		{
			FSlot test;
			test.Amount = Num;
			test.Item = Array[Index].Item;

			Array[Index] = test;

			
			return true;
		}
		else
		{		
			
			if (&Equipment==&Array)
			{
				ServerAddEquipVisual(false,Array[Index].Item, Index,this);
				if (Preview == nullptr) return false;
				Preview->RemoveEquipmentMesh(Index); // Parameter->  Index //해야 할 것
				
				MakeEmptySlot(Array, Index);
			}
			else
			{				
				MakeEmptySlot(Array, Index);
			}
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

void AMyInvenCharacter::MakeEmptySlot(TArray<FSlot>& Array, int32 Index)
{		
	if (ItemDataBase == nullptr) return;

	Array[Index].Item.DataTable = ItemDataBase;
	Array[Index].Item.RowName = "Empty";
	Array[Index].Amount = 0;
		
}

bool AMyInvenCharacter::RemoveItem(FSlot Item)
{
	bool bValue = false;
	int32 AmountValue = 0;
	int32 IndexValue=0;
	

	FindStack(Item, bValue, AmountValue, IndexValue);
	if (bValue)
	{
		if (AmountValue - Item.Amount >= 0)
		{
			if (AmountValue - Item.Amount == 0)
			{
				MakeEmptySlot(PlayerSlot, IndexValue);
				return true;
			}
			else
			{
				PlayerSlot[IndexValue].Item = Item.Item;
				PlayerSlot[IndexValue].Amount = AmountValue - Item.Amount;
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}

bool AMyInvenCharacter::AddItemToPlayerSlot(FSlot Item, int32 Index)
{
	if (PlayerSlot[Index].Item.RowName == "Empty")
	{
		PlayerSlot[Index] = Item;
		return true;
	}
	else
	{
		
		FName LocalRowName = Item.Item.RowName;
		FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName,"");

		if (OutRow != nullptr)
		{
			if (PlayerSlot[Index].Item.RowName == Item.Item.RowName && OutRow->CanStack)
			{
				FSlot SlotValue;
				SlotValue.Item = Item.Item;
				SlotValue.Amount = PlayerSlot[Index].Amount + Item.Amount;

				PlayerSlot[Index] = SlotValue;
				return true;
			}
							
		}
		return false;		
	}
}

bool AMyInvenCharacter::AddItemToEquipment(FSlot Item, int32 Index)
{
	FString LocalType = "";
	FName LocalRowName = Item.Item.RowName;
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName, "");

	if (OutRow != nullptr)
	{
		switch (Index)
		{
		case 0:
			LocalType = "Head";
			break;

		case 1:
			LocalType = "Top";
			break;

		case 2:
			LocalType = "Weapon";
			break;

		case 3:
			LocalType = "Shield";
			break;

		case 4:
			LocalType = "Weapon";
			break;
		

		default:
			return false;
			
		}
		
		if (OutRow->Type == LocalType)
		{
			if (Equipment[Index].Item.RowName == "Empty")
			{
				Equipment[Index] = Item;

				ServerAddEquipVisual(true, Item.Item, Index, this);
				Preview->SetEquipmentMesh(Item.Item, Index); // Parameter-> Item.Item , Index  /// 작성해야 할 것!
				return true;
			}
			else
			{
				if (Equipment[Index].Item.RowName == Item.Item.RowName && OutRow->CanStack)
				{
					FSlot SlotValue;
					SlotValue.Item = Item.Item;
					SlotValue.Amount = Equipment[Index].Amount + Item.Amount;

					Equipment[Index] = SlotValue;
					return true;
				}
				
			}
		}
	
	}
	return false;
	
}

bool AMyInvenCharacter::MoveItem(FSlot Item, FString Source, FString Direction, int32 ItemIndex, int32 SlotIndex)
{
	if (Direction == "Inventory")
	{
		if (IsValid(Backpack))
		{
			if (Backpack->AddItem(Item))
			{
				return SwitchString(Item, Source, Direction, ItemIndex);
			}
				
		}
		
	}
	else if (Direction == "Player")
	{
		if (AddItemToPlayerSlot(Item, SlotIndex))
		{
			return SwitchString(Item, Source, Direction, ItemIndex);
		}
	}
	else if (Direction == "Equipment")
	{
		if (AddItemToEquipment(Item, SlotIndex))
		{
			return SwitchString(Item, Source, Direction, ItemIndex);
		}
	}
	else if (Direction == "Shop")
	{
		return SwitchString(Item, Source, Direction, ItemIndex);
	}
	return false; // will be remove
}


bool AMyInvenCharacter::SwitchString(FSlot Item, FString Source, FString Direction, int32 ItemIndex)
{
	if (Source == "Inventory")
	{
		if (Backpack == nullptr) return false;
		if (Backpack->RemoveAmountAtIndex(ItemIndex, Item.Amount) && Direction == "Shop")
		{			
			return CalculateMoney(Item);
		}
	}
	else if (Source == "Player")
	{
		
		if (RemoveAmountAtIndex(ItemIndex, Item.Amount, PlayerSlot) && Direction == "Shop")
		{			
			return CalculateMoney(Item);
		}
	}
	else if (Source == "Equipment")
	{		
		if (RemoveAmountAtIndex(ItemIndex, Item.Amount, Equipment) && Direction == "Shop")
		{			
			return CalculateMoney(Item);
		}
	}
	else if (Source == "Shop")
	{
		return true;
	}
	return false;
}


bool AMyInvenCharacter::CalculateMoney(FSlot Item)
{
	FName LocalRowName = Item.Item.RowName;
	FInventory* OutRow = Item.Item.DataTable->FindRow<FInventory>(LocalRowName, "");
	if (OutRow != nullptr)
	{
		Money = Money + (OutRow->Value * Item.Amount);
		ServerSellItem(Interactable, Item);
		
		return true;
	}
	return false;
}

void AMyInvenCharacter::EquipmentVisual(bool IsEquiping, FDataTableRowHandle Item, int32 SlotIndex, AMyInvenCharacter* Player)
{
	if (Player == nullptr) return;

	FName SocketName;
	FName LocalRowName = Item.RowName;
	FInventory* OutRow = Item.DataTable->FindRow<FInventory>(LocalRowName, "");
	
	
	if (OutRow != nullptr)
	{
		if (IsEquiping)
		{
			if (OutRow->Actor == nullptr) return;

			AActor* SpawnedActor =GetWorld()->SpawnActor<AActor>(OutRow->Actor);
			if (SpawnedActor == nullptr) return;

			FEquipment Equip;
			Equip.Actor = SpawnedActor;
			Equip.Type = OutRow->Type;

			Player->EquipVisuals[SlotIndex] = Equip;

			switch (SlotIndex)
			{
			case 0:
				SocketName = "Head";
				break;

			case 1:
				SocketName = "Top";
				break;

			case 2:
				SocketName = "Weapon";
				break;

			case 3:
				SocketName = "Shield";
				break;

			case 4:
				SocketName = "BackEquip";
				break;

			
			default:
				break;
			}
			
			FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			SpawnedActor->AttachToComponent(Player->GetMesh(), TransformRules, SocketName);
			
		}
		else
		{
			Player->EquipVisuals[SlotIndex].Actor->Destroy();
		}
	}
}



void AMyInvenCharacter::RefreshInvWidget()
{
	RemoveWidget();
	if (WBPInvWidgetClass == NULL) return;
	MakeWidget(WBPInvWidgetClass);
	//FTimerDelegate TimerDel;	
	//TimerDel.BindUFunction(this, "MakeWidget", WBPInvWidgetClass);
	
	//GetWorld()->GetTimerManager().SetTimer(Handle, TimerDel, 0.1f, false);
}

void AMyInvenCharacter::RefreshShopWidget()
{
	RemoveWidget();
	if(WBPShopWidgetClass == NULL) return;
	MakeWidget(WBPShopWidgetClass);
}

void AMyInvenCharacter::MakeWidget(TSubclassOf<class UUserWidget> Widget)
{
	if (!IsValid(UI))
	{		
		if (Widget == NULL) return;
		UI = CreateWidget(GetWorld(), Widget);
		UI->AddToViewport();
		APlayerController* cont = UI->GetOwningPlayer();
		if (cont == nullptr) return;
		FInputModeGameAndUI Mode;
		
		cont->bShowMouseCursor = true;
		cont->SetInputMode(Mode);
		//GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
	else
	{		
		RemoveWidget();
		//GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
}


void AMyInvenCharacter::RemoveWidget()
{
	if (IsValid(UI))
	{
		APlayerController* cont = UI->GetOwningPlayer();
		if (cont == nullptr) return;
		FInputModeGameOnly Mode;
		cont->bShowMouseCursor = false;
		cont->SetInputMode(Mode);
		UI->RemoveFromParent();
		UI = nullptr;
	}
}

void AMyInvenCharacter::ServerDestroy_Implementation(AActor* ActorToDestroy)
{
	if (ActorToDestroy == nullptr) return;
	ActorToDestroy->Destroy();
}

void AMyInvenCharacter::ServerAttachBackpack_Implementation(AActor* Attachable, class USceneComponent* PlayerMesh, class UPrimitiveComponent* Component, FVector NewScale)
{
	if (Attachable == nullptr || PlayerMesh == nullptr || Component == nullptr) return;
	MultiAttachBackpack(Attachable, PlayerMesh, Component, NewScale);
}

void AMyInvenCharacter::MultiAttachBackpack_Implementation(AActor* Attachable, class USceneComponent* PlayerMesh, class UPrimitiveComponent* Component, FVector NewScale)
{
	if (Attachable == nullptr || PlayerMesh == nullptr || Component == nullptr) return;
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	Attachable->AttachToComponent(GetMesh(), TransformRules, "BackpackSocket");
	Component->SetWorldScale3D(NewScale);
	Component->SetSimulatePhysics(false);
}



void AMyInvenCharacter::ThrowBackpack()
{
	if (Backpack != nullptr)
	{
		
		FVector Loc = GetCapsuleComponent()->GetComponentLocation() + (GetCapsuleComponent()->GetForwardVector()*200.f);
		ServerSpawnBackpack(Backpack->PlayerSlot, Loc, Backpack);
		if (IsValid(UI))
		{
			RefreshInvWidget();
		}
	}
}

void AMyInvenCharacter::ServerSpawnBackpack_Implementation(const TArray<FSlot>& Array, FVector Location, AActor* Bag)
{
	if (Bag == nullptr) return;
	if (IsValid(BackpackClass))
	{		
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;

		FVector Loc = Location;
		FTransform Trans;
		Trans.SetLocation(Loc);
		Trans.SetScale3D(FVector(1.f));
		
		
				
		ABackpackEquipment* pack = GetWorld()->SpawnActor<ABackpackEquipment>(BackpackClass, Trans, Params);
		if (pack == nullptr) return;
		pack->PlayerSlot = Array;

		Bag->Destroy();
		
		
	}
}

void AMyInvenCharacter::ServerSpawnItem_Implementation(FTransform SpawnTrans, FSlot Slot)
{
	FActorSpawnParameters Params;
	Params.Instigator = GetInstigator();
	Params.Owner = this;
	if (PickupMasterClass == nullptr) return;
	APickup_Master* Pickup = GetWorld()->SpawnActor<APickup_Master>(PickupMasterClass, SpawnTrans, Params);
	if (Pickup == nullptr) return;
	Pickup->GetStaticMeshComponent()->SetSimulatePhysics(true);
	Pickup->Item = Slot;
	Pickup->ChangeMesh();
}

void AMyInvenCharacter::ToggleInventory()
{	
	if (WBPInvWidgetClass == NULL) return;
	MakeWidget(WBPInvWidgetClass);	
}

void AMyInvenCharacter::ServerAddEquipVisual_Implementation(bool Equiping, FDataTableRowHandle item, int32 index, AMyInvenCharacter* player)
{
	if (player == nullptr) return;
	MulAddEquipVisual(Equiping, item, index, player);
}

void AMyInvenCharacter::MulAddEquipVisual_Implementation(bool Equiping, FDataTableRowHandle item, int32 index, AMyInvenCharacter* player)
{
	if (player == nullptr) return;
	EquipmentVisual(Equiping, item, index, player);
}


void AMyInvenCharacter::ServerSellItem_Implementation(UObject* Target, FSlot Item)
{
	if (Target == nullptr) return;
	IAI_Interaction* Interface = Cast<IAI_Interaction>(Target);
	if (Interface == nullptr) return;

	Interface->SellItem(Item);
}

void AMyInvenCharacter::ServerBuyItem_Implementation(UObject* Target, int32 Index, FSlot Item)
{
	if (Target == nullptr) return;
	IAI_Interaction* Interface = Cast<IAI_Interaction>(Target);
	if (Interface == nullptr) return;

	Interface->BuyItem(Item, Index);
}

void AMyInvenCharacter::SpawnPreviewActor()
{
	if (PreviewActorClass == NULL) return;
	FActorSpawnParameters Params;
	Params.Instigator = GetInstigator();
	Params.Owner = this;

	FVector Loc = FVector(10000000.0f, 0.f, 0.f);
	FRotator Rot = FRotator(0.0f, 0.0f, 0.0f);
	
	Preview = Cast<APreviewActor>(GetWorld()->SpawnActor<AActor>(PreviewActorClass, Loc, Rot , Params));
	
	if (Preview != nullptr)
	{
		
		Preview->Mesh->SetSkeletalMesh(GetMesh()->SkeletalMesh); 
		Preview->Mesh->SetAnimInstanceClass(GetMesh()->GetAnimClass());
		Preview->SetMeshLocation(EquipVisuals);
	}
}