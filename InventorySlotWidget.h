<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Input/Reply.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "Item.h"
#include "RPG_cppCharacter.h"
#include "RPGPlayerController.h"
#include "InventoryTooltipWidget.h"

#include "InventorySlotWidget.generated.h"

// forward
class UInventoryWindowWidget;
class AContainer;
class UInventoryComponent;

UCLASS()
class RPG_CPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	uint32 slotIndex;

	UPROPERTY(EditAnywhere)
	FSlotStruct slotContent;

	UPROPERTY(EditAnywhere)
	UInventoryTooltipWidget* item_tooltip;

	UPROPERTY()
	UInventoryWindowWidget* inventory_window;

	FString getItemQuantity();

	void setParams(FSlotStruct p_slotContent, uint32 p_slotIndex, UInventoryWindowWidget* p_inventory_window);

	UFUNCTION()
	void onButtonClicked();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual FReply NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent) override;

	virtual void NativeOnFocusLost(const FFocusEvent & InFocusEvent) override;

	//virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Input/Reply.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "Item.h"
#include "RPG_cppCharacter.h"
#include "RPGPlayerController.h"
#include "InventoryTooltipWidget.h"

#include "InventorySlotWidget.generated.h"

// forward
class UInventoryWindowWidget;
class AContainer;
class UInventoryComponent;

UCLASS()
class RPG_CPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	uint32 slotIndex;

	UPROPERTY(EditAnywhere)
	FSlotStruct slotContent;

	UPROPERTY(EditAnywhere)
	UInventoryTooltipWidget* item_tooltip;

	UPROPERTY()
	UInventoryWindowWidget* inventory_window;

	FString getItemQuantity();

	void setParams(FSlotStruct p_slotContent, uint32 p_slotIndex, UInventoryWindowWidget* p_inventory_window);

	UFUNCTION()
	void onButtonClicked();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual FReply NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent) override;

	virtual void NativeOnFocusLost(const FFocusEvent & InFocusEvent) override;

	//virtual FReply NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent) override;
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
