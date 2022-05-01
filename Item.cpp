// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventoryComponent.h"


void AItem::interact_Implementation(AActor* interactor)
{
	// get player inventory
	TSubclassOf<UActorComponent> UInventoryComponent_Class;
	TArray <UInventoryComponent*> comps;
	interactor->GetComponents<UInventoryComponent>(comps);
	UInventoryComponent* inventoryComponent = (UInventoryComponent*)comps[0];

	// create slot structure from item structure
	FSlotStruct slotStruct = { itemStruct, 1 };

	// add slot structure to inventory
	inventoryComponent->addToInventory(slotStruct);
	isInteracting = true;

	// delay

	//isInteracting = false;

	Destroy();
}

bool AItem::useItem()
{
	return true;
}

// Sets default values
AItem::AItem(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	SetRootComponent(root);

	isInteracting = false;

	focusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FocusWidgetComponent"));
	focusWidgetComponent->SetDrawAtDesiredSize(true);
	focusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	focusWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	focusWidgetComponent->SetupAttachment(root, FName("FocusWidgetComponent"));

	itemStruct.item_ref = this;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

