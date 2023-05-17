// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomStruct.h"
#include "PickupInterection.h"
#include "MyInvenCharacter.generated.h"

UCLASS(config=Game)
class AMyInvenCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyInvenCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(Replicated)
	TArray<FSlot> PlayerSlot;

	UPROPERTY(Replicated)
	TArray<FSlot> Equipment; 

	UPROPERTY(Replicated)
	TArray<FEquipment> EquipVisuals;

	UPROPERTY()
	class UUserWidget* UI;

	UPROPERTY(Replicated)
	class AActor* Interactable;

	UFUNCTION()
	void SetMoney(int32 New) { Money = New; }

	UFUNCTION(BlueprintPure)
	int32 GetMoney() { return Money; }

	UPROPERTY(Replicated)
	class ABackpackEquipment* Backpack;

	UFUNCTION()
	bool AddItem(FSlot Item);

	UFUNCTION()
	void FindStack(FSlot Slot, bool& Found, int32& Amount, int32& Index);

	UFUNCTION()
	void FindEmptySloy(bool& Success, int32& Index);

	UFUNCTION()
	bool RemoveAmountAtIndex(int32 Index, int32 Amount, TArray<FSlot>& Array);

	UFUNCTION()
	bool RemoveItem(FSlot Item);

	UFUNCTION()
	bool AddItemToPlayerSlot(FSlot Item, int32 Index);

	UFUNCTION()
	bool AddItemToEquipment(FSlot Item, int32 Index);

	UFUNCTION()
	bool MoveItem(FSlot Item, FString Source, FString Direction, int32 ItemIndex, int32 SlotIndex);

	UFUNCTION()
	bool SwitchString(FSlot Item, FString Source, FString Direction, int32 ItemIndex);

	UFUNCTION()
	bool CalculateMoney(FSlot Item);

	UFUNCTION()
	void EquipmentVisual(bool IsEquiping, FDataTableRowHandle Item, int32 SlotIndex, AMyInvenCharacter* Player);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WBPInvWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WBPShopWidgetClass;

	UFUNCTION()
	void MakeWidget(TSubclassOf<class UUserWidget> Widget);

	UFUNCTION()
	void RemoveWidget();

	UFUNCTION()
	void RefreshInvWidget();

	UFUNCTION()
	void RefreshShopWidget();

	UFUNCTION(Server, Reliable)
	void ServerDestroy(AActor* ActorToDestroy);

	UFUNCTION(Server, Reliable)
	void ServerAttachBackpack(AActor* Attachable, class USceneComponent* PlayerMesh, class UPrimitiveComponent* Component, FVector NewScale);

	UFUNCTION(NetMulticast, Reliable)
	void MultiAttachBackpack(AActor* Attachable, class USceneComponent* PlayerMesh, class UPrimitiveComponent* Component, FVector NewScale);

	UFUNCTION(Server, Reliable)
	void ServerSpawnBackpack(const TArray<FSlot>& Array, FVector Location,  AActor* Bag);

	UFUNCTION(Server, Reliable)
	void ServerSpawnItem(FTransform SpawnTrans,  FSlot Slot);

	UFUNCTION(Server, Reliable)
	void ServerAddEquipVisual(bool Equiping, FDataTableRowHandle item, int32 index , AMyInvenCharacter* player);

	UFUNCTION(NetMulticast, Reliable)
	void MulAddEquipVisual(bool Equiping, FDataTableRowHandle item, int32 index, AMyInvenCharacter* player);

	UFUNCTION(Server, Reliable)
	void ServerSellItem(UObject* Target , FSlot Item);

	UFUNCTION(Server, Reliable)
	void ServerBuyItem(UObject* Target,  int32 Index, FSlot Item);

	UFUNCTION()
	void ThrowBackpack();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABackpackEquipment> BackpackClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APickup_Master> PickupMasterClass;

	UFUNCTION()
	void ToggleInventory();

	UPROPERTY()
	class UDataTable* ItemDataBase;

	bool IsShopOpen;

	UPROPERTY(EditAnywhere ,BlueprintReadOnly)
	class UMaterialInstanceDynamic* RenderTargetMat;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PreviewActorClass;

	UFUNCTION()
	void SpawnPreviewActor();

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	

private:
	int32 Money;

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void MakeEmptySlot(TArray<FSlot>& Array, int32 Index);

	UFUNCTION()
	void SetPlayerSlot(TArray<FSlot>& Slot, int32 Int);

	FTimerHandle Handle;
	
	class APreviewActor* Preview;
};

