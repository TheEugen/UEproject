// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"
#include "InventoryComponent.h"

void AContainer::interact_Implementation(AActor* interactor)
{
	inventoryComponent->toggleInventory(FAnchors(0.2, 0.1, 0.8, 0.8));
}

// Sets default values
AContainer::AContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	staticMesh->SetStaticMesh(SphereMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	staticMesh->SetMaterial(0, (UMaterial*)Material.Object);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetupAttachment(root, FName("StaticMesh"));

	boxCollision = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision"));
	boxCollision->SetupAttachment(root, FName("BoxCollision"));
	
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	AddOwnedComponent(inventoryComponent);

	focusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FocusWidgetComponent"));
	focusWidgetComponent->SetDrawAtDesiredSize(true);
	focusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	focusWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	focusWidgetComponent->SetupAttachment(root, FName("FocusWidgetComponent"));
}

// Called when the game starts or when spawned
void AContainer::BeginPlay()
{
	// fill up inventory
	for(int i = 0; i < items.Num(); ++i)
	{
		AItem* item = (AItem*)items[i].item->GetDefaultObject();
		FSlotStruct slot = { item->itemStruct, items[i].quantity };
		inventoryComponent->addToInventory(slot);
	}

	Super::BeginPlay();
}

// Called every frame
void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

