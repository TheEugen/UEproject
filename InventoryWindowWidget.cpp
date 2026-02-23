// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWindowWidget.h"
#include "InventorySlotWidget.h"
#include "Components/GridSlot.h"
#include "Container.h"
#include "InventoryComponent.h"
#include "RPG_cppCharacter.h"


FReply UInventoryWindowWidget::NativeOnKeyDown(const FGeometry & InGeometry, const FKeyEvent & InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> action_mapping;

	AActor* inventory_owner = inventoryComponent->GetOwner();

	FName interaction_name;

	// determine if we react to inventory ctrl or interact ctrl
	if( inventory_owner->IsA(AContainer::StaticClass()) ) 
		interaction_name = FName("Interact");
	else if( inventory_owner->IsA(ARPG_cppCharacter::StaticClass()) )
		interaction_name = FName("Inventory");
	else
		UE_LOG(LogTemp, Fatal, TEXT("InventoryWindowWidget: inventory owner is neither pc or container"));

	action_mapping = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(interaction_name);

	for(FInputActionKeyMapping key : action_mapping)
	{
		if( InKeyEvent.GetKey() == key.Key )
		{
			inventoryComponent->toggleInventory(FAnchors());

			for(UInventorySlotWidget* slot : inventorySlots)
			{
				if( slot->item_tooltip )
					slot->item_tooltip->RemoveFromParent();
			}

			return FReply::Handled();
		}
	}

	return FReply::Handled();
}

void UInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// get the grid panel and inventory title text
	TArray<UWidget*> childWidgets;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, childWidgets);
	UTextBlock* inventoryTitleWidget = nullptr;
	for(UWidget* w : childWidgets)
	{
		if( w->IsA(UGridPanel::StaticClass()) )
		{
			gridPanel = (UGridPanel*) w;
			if( inventoryTitleWidget )
				break;
			continue;
		}

		if( w->IsA(UTextBlock::StaticClass()) )
		{
			inventoryTitleWidget = (UTextBlock*) w;
			if( gridPanel )
				break;
			continue;
		}
	}

	// set inventory name
	inventoryTitleWidget->SetText(FText::FromString(inventoryComponent->InventoryName));

	// get inventory slot widget class reference
	FStringClassReference InventorySlotClassRef(TEXT("/Game/ThirdPersonCPP/UI/Inventory/InventorySlotWidget.InventorySlotWidget_C"));
	UClass* InventorySlotClass = InventorySlotClassRef.TryLoadClass<UUserWidget>();

	// initialize inventorySlots array
	inventorySlots.SetNum(inventoryComponent->slotCount, false);

	// player controller reference
	ARPGPlayerController* playerController = (ARPGPlayerController*)UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);

	// create the slot widgets
	for(uint32 i = 0; i < inventoryComponent->slotCount; ++i)
	{
		if ( InventorySlotClass )
		{	
			inventorySlots[i] = (UInventorySlotWidget*) CreateWidget<UUserWidget>(this, InventorySlotClass, FName(TEXT("InventorySlot"), i));
			inventorySlots[i]->setParams(inventoryComponent->Inventory[i], i, this);
			gridPanel->AddChildToGrid(inventorySlots[i], i / gridPanel->ColumnFill.Num(), i % gridPanel->ColumnFill.Num());
		} 
	}

	// set keyboard focus to inventory
	SetKeyboardFocus();

	// set keyboard focus to the first slot
	//inventorySlots[0]->SetKeyboardFocus();
}