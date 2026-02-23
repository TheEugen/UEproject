// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObjectGlobals.h"
#include "InventorySlotWidget.h"
#include "Components/GridPanel.h"
#include "Input/Reply.h"
#include "GameFramework/PlayerInput.h"


#include "InventoryWindowWidget.generated.h"

// Forward
class UInventoryComponent;
class AContainer;
class ARPG_cppCharacter;

UCLASS()
class RPG_CPP_API UInventoryWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	UGridPanel* gridPanel;

	UPROPERTY(EditAnywhere)
	UInventoryComponent* inventoryComponent;

	UPROPERTY(EditAnywhere)
	FString title;

	UPROPERTY(VisibleAnywhere)
	TArray<UInventorySlotWidget*> inventorySlots;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;

};
