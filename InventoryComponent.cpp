// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Item.h"
#include "InventoryWindowWidget.h"


void UInventoryComponent::setEquippedWeapon(AItem_Weapon* item_weapon)
{
	equipped_weapon = item_weapon;
}

void UInventoryComponent::unequipWeapon()
{
	setEquippedWeapon(nullptr);
}

void UInventoryComponent::equipWeapon(AItem_Weapon* item_weapon)
{
	setEquippedWeapon(item_weapon);
}

AItem_Weapon* UInventoryComponent::getEquippedWeapon()
{
	return equipped_weapon;
}

void UInventoryComponent::markSocketAsAvailable(AItem_Weapon::AttachPoint attach_point, bool available)
{
	for(AItem_Weapon::AttachInfo ai : socket_states)
	{
		if( ai.attach_point == attach_point )
		{
			ai.inUse = !available;
			break;
		}		
	}
}

bool UInventoryComponent::findFreeSocket(AItem_Weapon* item_weapon, AItem_Weapon::AttachPoint& attach_point)
{
	bool found = false;

	for(AItem_Weapon::AttachPoint ap : item_weapon->getAttachPoints() )
	{
		for(AItem_Weapon::AttachInfo ai : socket_states)
		{
			// socket isnt used and is matching point type
			if( !ai.inUse && ap == ai.attach_point )
			{
				found = true;
				attach_point = ap;
				break;
			}
		}
	}

	return found;
}

bool UInventoryComponent::inventoryHasSpace()
{
	// check if we have a empty slot for our item
	for(int i = 0; i < Inventory.Num(); ++i)
	{
		if( Inventory[i].quantity <= 0 )
			return true;
	}

	return false;
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	pInventoryWindow = nullptr;

	Inventory.SetNum(slotCount, false);

	socket_states.Add(AItem_Weapon::AttachInfo(false, AItem_Weapon::AttachPoint::Right, nullptr));
	socket_states.Add(AItem_Weapon::AttachInfo(false, AItem_Weapon::AttachPoint::Left, nullptr));
	socket_states.Add(AItem_Weapon::AttachInfo(false, AItem_Weapon::AttachPoint::Back, nullptr));
}

bool UInventoryComponent::createMaxStack(FSlotStruct item)
{
	if( !inventoryHasSpace() )
		return false;

	// create stack in first empty slotstructure
	for(int i = 0; i < Inventory.Num(); ++i)
	{
		if( Inventory[i].quantity == 0 )
		{
			Inventory[i].itemStruct = item.itemStruct;
			Inventory[i].quantity = item.itemStruct.maxStackSize;
			break;
		}
	}

	return true;
}

bool UInventoryComponent::addToInventory(FSlotStruct itemToAdd)
{
	int index;
	bool success = false;

	// item is stackable
	if( itemToAdd.itemStruct.isStackable )
	{
		// item has a partial stack in our inventory
		index = hasPartialStack(itemToAdd);

		// item has a stack
		if( index > -1)
		{
			success = addToStack(itemToAdd, index);
		}
		// item has no stack
		else
		{
			// case: add quantity > maximum stack size
			if( itemToAdd.quantity > itemToAdd.itemStruct.maxStackSize )
			{
				int whole = itemToAdd.quantity / itemToAdd.itemStruct.maxStackSize;
				int remainder = itemToAdd.quantity % itemToAdd.itemStruct.maxStackSize;

				// create quantity/maxStacksize stacks
				for(int i = 0; i < whole; ++i)
					success = createMaxStack(itemToAdd);

				// create leftover stack
				if( remainder > 0 )
					success = createStack(itemToAdd, remainder);
			}
			else
			{
				success = createStack(itemToAdd);
			}
		}
	}
	// item is not stackable
	else
	{
		success = createStack(itemToAdd);
	}

	return success;
}

int UInventoryComponent::hasPartialStack(FSlotStruct item)
{
	uint32 index = 0;
	bool hasStack = false;
	for(FSlotStruct s: Inventory)
	{
		if( s.itemStruct.itemClass == item.itemStruct.itemClass && s.quantity < item.itemStruct.maxStackSize )
		{
			hasStack = true;
			break;
		}

		++index;
	}

	if( hasStack )
		return index;
	else
		return -1;
}

bool UInventoryComponent::createStack(FSlotStruct item, uint32 quantity)
{
	if( !inventoryHasSpace() )
		return false;

	// find first empty slot
	for(int i = 0; i < Inventory.Num(); ++i)
	{
		if( Inventory[i].quantity == 0 )
		{
			Inventory[i].itemStruct = item.itemStruct;
			Inventory[i].quantity = quantity;
			break;
		}	
	}

	return true;
}


bool UInventoryComponent::createStack(FSlotStruct item)
{
	if( !inventoryHasSpace() )
		return false;

	// find first empty slot
	for(int i = 0; i < Inventory.Num(); ++i)
	{
		if( Inventory[i].quantity == 0 )
		{
			Inventory[i].itemStruct = item.itemStruct;
			Inventory[i].quantity = item.quantity;
			break;
		}	
	}

	return true;
}

bool UInventoryComponent::addToStack(FSlotStruct item, uint32 index)
{
	// adding would overflow the item stack
	if( Inventory[index].quantity + item.quantity > Inventory[index].itemStruct.maxStackSize )
	{
		// TODO: case when adding would overflow and create more than 1 stack
		uint32 remainder = item.itemStruct.maxStackSize - Inventory[index].quantity;
		Inventory[index].quantity = item.itemStruct.maxStackSize;
		createStack(item, remainder);
	}
	// common adding
	else
		Inventory[index].quantity += item.quantity;

	// fail possible?
	return true;
}

void UInventoryComponent::inventoryQuery() {}

void UInventoryComponent::toggleInventory(FAnchors anchors)
{
	if( pInventoryWindow )
	{
		pInventoryWindow->RemoveFromParent();
		pInventoryWindow = nullptr;
		ARPGPlayerController* playerController = (ARPGPlayerController *)UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);
		FInputModeGameOnly inputModeGameOnly = FInputModeGameOnly();
		playerController->SetInputMode(inputModeGameOnly);
	}
	else
	{
		FStringClassReference InventoryWindowClassRef(TEXT("/Game/ThirdPersonCPP/UI/Inventory/InventoryWindowWidget.InventoryWindowWidget_C"));
		if ( UClass* InventoryWindowClass = InventoryWindowClassRef.TryLoadClass<UUserWidget>() )
		{
			ARPGPlayerController* playerController = (ARPGPlayerController *)UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);
			pInventoryWindow = (UInventoryWindowWidget*)CreateWidget<UUserWidget>(playerController, InventoryWindowClass, FName("InventoryWindow"));
			pInventoryWindow->inventoryComponent = this;
			ui_canvas = (UCanvasPanel*) playerController->pUIWidget->WidgetTree->RootWidget;
			ui_canvasSlot = ui_canvas->AddChildToCanvas(pInventoryWindow);
			ui_canvasSlot->bAutoSize = false;
			ui_canvasSlot->SetAlignment(FVector2D(0.0, 0.0));
			ui_canvasSlot->SetAnchors(anchors);
			ui_canvasSlot->SetPosition(Position);

			FInputModeUIOnly inputModeUIonly = FInputModeUIOnly();
			playerController->SetInputMode(inputModeUIonly);		
			playerController->SetShowMouseCursor(false);
		} 
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

