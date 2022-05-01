// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Layout/Anchors.h"
#include "Engine/World.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"


#include "RPGPlayerController.h"
#include "Item.h"
#include "Item_Weapon.h"

#include "InventoryComponent.generated.h"

// forward
class AItem;
class UInventoryWindowWidget;


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_CPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	bool inventoryHasSpace();
	bool findFreeSocket(AItem_Weapon* item_weapon, AItem_Weapon::AttachPoint& attach_point);
	void markSocketAsAvailable(AItem_Weapon::AttachPoint attach_point, bool available);

public:
	UPROPERTY(EditAnywhere)
	FString InventoryName;

	UPROPERTY(VisibleAnywhere)
	TArray<FSlotStruct> Inventory;

	UPROPERTY()
	FVector2D Position;

	UPROPERTY(EditAnywhere)
	uint32 slotCount = 24;

	UPROPERTY()
	UInventoryWindowWidget* pInventoryWindow;

	UPROPERTY()
	UCanvasPanel* ui_canvas;

	UPROPERTY()
	UCanvasPanelSlot* ui_canvasSlot;

	UPROPERTY()
	AItem_Weapon* equipped_weapon;

	TArray<AItem_Weapon::AttachInfo> socket_states;
	
	// Sets default values for this component's properties
	UInventoryComponent();

	void toggleInventory(FAnchors anchors);

	bool addToInventory(FSlotStruct itemToAdd);

	void equipWeapon(AItem_Weapon* item_weapon);

	void unequipWeapon();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	bool createStack(FSlotStruct item);
	bool createStack(FSlotStruct item, uint32 quantity);
	bool createMaxStack(FSlotStruct item);
	// no partial stack -> -1 return
	int hasPartialStack(FSlotStruct item);
	bool addToStack(FSlotStruct item, uint32 index);
	void inventoryQuery();

	void setEquippedWeapon(AItem_Weapon* item_weapon);
	AItem_Weapon* getEquippedWeapon();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
