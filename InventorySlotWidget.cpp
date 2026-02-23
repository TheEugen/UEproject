<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryWindowWidget.h"
#include "InventoryComponent.h"
#include "Container.h"

/*
FReply UInventorySlotWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> action_mapping;

	AActor* inventory_owner = inventory_window->inventoryComponent->GetOwner();

	if( inventory_owner->IsA(AContainer::StaticClass()) ) 
		action_mapping = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(FName("Interact"));
	else if( inventory_owner->IsA(ARPG_cppCharacter::StaticClass()) )
		action_mapping = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(FName("Inventory"));
	else
		UE_LOG(LogTemp, Fatal, TEXT("InventoryWindowWidget: inventory owner is neither pc or container"));

	for(FInputActionKeyMapping key : action_mapping)
	{
		if( InKeyEvent.GetKey() == key.Key )
		{
			inventory_window->inventoryComponent->toggleInventory(FAnchors());
			if( item_tooltip )
			{
				item_tooltip->RemoveFromParent();
				item_tooltip = nullptr;
			}
			return FReply::Handled();
		}
	}

	return FReply::Handled();
}*/

void UInventorySlotWidget::NativeOnFocusLost(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	if( item_tooltip )
	{
		item_tooltip->RemoveFromParent();
		item_tooltip = nullptr;
	}
}

FReply UInventorySlotWidget::NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	
	if( slotContent.quantity > 0 )
	{
		// get inventory tooltip widget class reference
		FStringClassReference inventoryTooltipClassRef(TEXT("/Game/ThirdPersonCPP/UI/Inventory/InventoryTooltipWidget.InventoryTooltipWidget_C"));
		if( UClass* inventoryTooltipClass = inventoryTooltipClassRef.TryLoadClass<UUserWidget>() )
		{
			item_tooltip = (UInventoryTooltipWidget*)CreateWidget<UUserWidget>(this, inventoryTooltipClass, FName("ItemTooltip"));
		
			// TODO engineer better
			// get the textblock objs
			TArray<UTextBlock*> textblocks;
			TArray<UWidget*> childs;
			item_tooltip->WidgetTree->GetChildWidgets(item_tooltip->WidgetTree->RootWidget, childs);
			for(UWidget* child : childs )
			{
				if( child->IsA(UTextBlock::StaticClass()) )
				{
					textblocks.Add((UTextBlock*) child);
					continue;
				}
			}

			// filling the text blocks with item information
			textblocks[0]->SetText(FText::FromString(slotContent.itemStruct.name));
			//textblocks[1]->SetText(FText::FromString(slotContent.itemStruct.));
			textblocks[2]->SetText(FText::FromString(slotContent.itemStruct.description));
			textblocks[4]->SetText(FText::FromString(FString::FromInt(slotContent.itemStruct.consume)));
			textblocks[5]->SetText(FText::FromString(FString::FromInt(slotContent.itemStruct.value)));

			// show the tooltip
			ARPGPlayerController* pc = (ARPGPlayerController*) GetWorld()->GetFirstPlayerController();
			UCanvasPanel* ui_canvas = (UCanvasPanel*) pc->pUIWidget->WidgetTree->RootWidget;
			UCanvasPanelSlot* ui_canvasslot = ui_canvas->AddChildToCanvas(item_tooltip);
			ui_canvasslot->bAutoSize = true;
			ui_canvasslot->SetAnchors(FAnchors(0.25f, 0.6f, 0.75f, 0.9f));
		}
	}

	return FReply::Handled();
}

void UInventorySlotWidget::setParams(FSlotStruct p_slotContent, uint32 p_slotIndex, UInventoryWindowWidget* p_inventory_window)
{
	slotContent = p_slotContent;
	slotIndex = p_slotIndex;
	this->inventory_window = p_inventory_window;
	bIsFocusable = true;

	// MOVE TO NATIVECONSTRUCT

	// get image and text widgets (item thumbnail and quantity)
	TArray<UWidget*> widgets;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, widgets);
	UImage* imageWidget = nullptr;
	UTextBlock* textWidget = nullptr;
	for(UWidget* w: widgets)
	{
		if( w->IsA(UImage::StaticClass()) )
		{
			imageWidget = (UImage*)w;
			if( textWidget )
				break;
			continue;
		}
		else if( w->IsA(UTextBlock::StaticClass()) )
		{
			textWidget = (UTextBlock*)w;
			if( imageWidget )
				break;
			continue;
		}
	}

	// set the thumbnail
	imageWidget->SetBrushFromTexture(slotContent.itemStruct.thumbnail, true);

	// set quantity text
	if( slotContent.quantity > 0)
		textWidget->SetText(FText::FromString(FString::FromInt(slotContent.quantity)));
	else
		textWidget->SetText(FText::FromString(TEXT("")));
}

FString UInventorySlotWidget::getItemQuantity()
{
	return FString::FromInt(slotContent.quantity);
}

UFUNCTION()
void UInventorySlotWidget::onButtonClicked()
{
	// check if we click a empty slot
	if( slotContent.quantity > 0 )
	{
		// check if clicked item is weapon
		// TODO: obj which item_ref points to get destroyed after some time, replace with attribute item_class // useable
		if( slotContent.itemStruct.item_ref->IsA(AItem_Weapon::StaticClass()) )
		{
			AItem_Weapon* weapon = (AItem_Weapon*) slotContent.itemStruct.item_ref;
		
			// we unequip the weapon
			if( weapon->getIsEquipped() )
			{
				weapon->unequip();
				inventory_window->inventoryComponent->unequipWeapon();
			}
			// we equip the weapon
			else
			{
				if( inventory_window->inventoryComponent->equipped_weapon )
					inventory_window->inventoryComponent->equipped_weapon->unequip();

				weapon->equip();
				inventory_window->inventoryComponent->equipWeapon(weapon);
			}
		
		}
	}
}

void UInventorySlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// get the slot button
	TArray<UWidget*> childs;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, childs);
	for(UWidget* w : childs)
	{
		if( w->IsA(UButton::StaticClass()) )
		{
			((UButton*) w)->OnClicked.RemoveDynamic(this, &UInventorySlotWidget::onButtonClicked);
			break;
		}		
	}
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// get the slot button
	TArray<UWidget*> childs;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, childs);
	for(UWidget* w : childs)
	{
		if( w->IsA(UButton::StaticClass()) )
		{
			((UButton*) w)->OnClicked.AddDynamic(this, &UInventorySlotWidget::onButtonClicked);
			break;
		}		
	}
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "InventoryWindowWidget.h"
#include "InventoryComponent.h"
#include "Container.h"

/*
FReply UInventorySlotWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> action_mapping;

	AActor* inventory_owner = inventory_window->inventoryComponent->GetOwner();

	if( inventory_owner->IsA(AContainer::StaticClass()) ) 
		action_mapping = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(FName("Interact"));
	else if( inventory_owner->IsA(ARPG_cppCharacter::StaticClass()) )
		action_mapping = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(FName("Inventory"));
	else
		UE_LOG(LogTemp, Fatal, TEXT("InventoryWindowWidget: inventory owner is neither pc or container"));

	for(FInputActionKeyMapping key : action_mapping)
	{
		if( InKeyEvent.GetKey() == key.Key )
		{
			inventory_window->inventoryComponent->toggleInventory(FAnchors());
			if( item_tooltip )
			{
				item_tooltip->RemoveFromParent();
				item_tooltip = nullptr;
			}
			return FReply::Handled();
		}
	}

	return FReply::Handled();
}*/

void UInventorySlotWidget::NativeOnFocusLost(const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	if( item_tooltip )
	{
		item_tooltip->RemoveFromParent();
		item_tooltip = nullptr;
	}
}

FReply UInventorySlotWidget::NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	
	if( slotContent.quantity > 0 )
	{
		// get inventory tooltip widget class reference
		FStringClassReference inventoryTooltipClassRef(TEXT("/Game/ThirdPersonCPP/UI/Inventory/InventoryTooltipWidget.InventoryTooltipWidget_C"));
		if( UClass* inventoryTooltipClass = inventoryTooltipClassRef.TryLoadClass<UUserWidget>() )
		{
			item_tooltip = (UInventoryTooltipWidget*)CreateWidget<UUserWidget>(this, inventoryTooltipClass, FName("ItemTooltip"));
		
			// TODO engineer better
			// get the textblock objs
			TArray<UTextBlock*> textblocks;
			TArray<UWidget*> childs;
			item_tooltip->WidgetTree->GetChildWidgets(item_tooltip->WidgetTree->RootWidget, childs);
			for(UWidget* child : childs )
			{
				if( child->IsA(UTextBlock::StaticClass()) )
				{
					textblocks.Add((UTextBlock*) child);
					continue;
				}
			}

			// filling the text blocks with item information
			textblocks[0]->SetText(FText::FromString(slotContent.itemStruct.name));
			//textblocks[1]->SetText(FText::FromString(slotContent.itemStruct.));
			textblocks[2]->SetText(FText::FromString(slotContent.itemStruct.description));
			textblocks[4]->SetText(FText::FromString(FString::FromInt(slotContent.itemStruct.consume)));
			textblocks[5]->SetText(FText::FromString(FString::FromInt(slotContent.itemStruct.value)));

			// show the tooltip
			ARPGPlayerController* pc = (ARPGPlayerController*) GetWorld()->GetFirstPlayerController();
			UCanvasPanel* ui_canvas = (UCanvasPanel*) pc->pUIWidget->WidgetTree->RootWidget;
			UCanvasPanelSlot* ui_canvasslot = ui_canvas->AddChildToCanvas(item_tooltip);
			ui_canvasslot->bAutoSize = true;
			ui_canvasslot->SetAnchors(FAnchors(0.25f, 0.6f, 0.75f, 0.9f));
		}
	}

	return FReply::Handled();
}

void UInventorySlotWidget::setParams(FSlotStruct p_slotContent, uint32 p_slotIndex, UInventoryWindowWidget* p_inventory_window)
{
	slotContent = p_slotContent;
	slotIndex = p_slotIndex;
	this->inventory_window = p_inventory_window;
	bIsFocusable = true;

	// MOVE TO NATIVECONSTRUCT

	// get image and text widgets (item thumbnail and quantity)
	TArray<UWidget*> widgets;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, widgets);
	UImage* imageWidget = nullptr;
	UTextBlock* textWidget = nullptr;
	for(UWidget* w: widgets)
	{
		if( w->IsA(UImage::StaticClass()) )
		{
			imageWidget = (UImage*)w;
			if( textWidget )
				break;
			continue;
		}
		else if( w->IsA(UTextBlock::StaticClass()) )
		{
			textWidget = (UTextBlock*)w;
			if( imageWidget )
				break;
			continue;
		}
	}

	// set the thumbnail
	imageWidget->SetBrushFromTexture(slotContent.itemStruct.thumbnail, true);

	// set quantity text
	if( slotContent.quantity > 0)
		textWidget->SetText(FText::FromString(FString::FromInt(slotContent.quantity)));
	else
		textWidget->SetText(FText::FromString(TEXT("")));
}

FString UInventorySlotWidget::getItemQuantity()
{
	return FString::FromInt(slotContent.quantity);
}

UFUNCTION()
void UInventorySlotWidget::onButtonClicked()
{
	// check if we click a empty slot
	if( slotContent.quantity > 0 )
	{
		// check if clicked item is weapon
		// TODO: obj which item_ref points to get destroyed after some time, replace with attribute item_class // useable
		if( slotContent.itemStruct.item_ref->IsA(AItem_Weapon::StaticClass()) )
		{
			AItem_Weapon* weapon = (AItem_Weapon*) slotContent.itemStruct.item_ref;
		
			// we unequip the weapon
			if( weapon->getIsEquipped() )
			{
				weapon->unequip();
				inventory_window->inventoryComponent->unequipWeapon();
			}
			// we equip the weapon
			else
			{
				if( inventory_window->inventoryComponent->equipped_weapon )
					inventory_window->inventoryComponent->equipped_weapon->unequip();

				weapon->equip();
				inventory_window->inventoryComponent->equipWeapon(weapon);
			}
		
		}
	}
}

void UInventorySlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// get the slot button
	TArray<UWidget*> childs;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, childs);
	for(UWidget* w : childs)
	{
		if( w->IsA(UButton::StaticClass()) )
		{
			((UButton*) w)->OnClicked.RemoveDynamic(this, &UInventorySlotWidget::onButtonClicked);
			break;
		}		
	}
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// get the slot button
	TArray<UWidget*> childs;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, childs);
	for(UWidget* w : childs)
	{
		if( w->IsA(UButton::StaticClass()) )
		{
			((UButton*) w)->OnClicked.AddDynamic(this, &UInventorySlotWidget::onButtonClicked);
			break;
		}		
	}
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
}