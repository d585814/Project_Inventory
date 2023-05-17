// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InvPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYINVEN_API UUI_InvPanel : public UUserWidget
{
	GENERATED_BODY()

public:	

	void NativeConstruct() override;

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BackpackText;
	
	UFUNCTION()
	FText IsGetBackpack();

	UPROPERTY()
	TSubclassOf<UUserWidget> ItemWidgetClass;

	void RefreshWidget(class AMyInvenCharacter* Cha);
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
private:
	int32 SlotPerRow=3;

	
		
};
