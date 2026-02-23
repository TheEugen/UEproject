<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Miscellaneous.h"

AItem_Miscellaneous::AItem_Miscellaneous(const FObjectInitializer& OI) : Super(OI)
{
	//??
	//Super::AItem();

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	staticMesh->SetStaticMesh(SphereMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	staticMesh->SetMaterial(0, (UMaterial*)Material.Object);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMesh->SetupAttachment(root, FName("StaticMesh"));

	focusWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, FVector(staticMesh->GetStaticMesh()->GetBounds().GetBox().GetSize()).Z * 1.5f));
	UE_LOG(LogTemp, Warning, TEXT("ITEM_MISC getBox getSize: %s"), *staticMesh->GetStaticMesh()->GetBounds().GetBox().GetSize().ToString());
	UE_LOG(LogTemp, Warning, TEXT("ITEM_MISC getBox max: %s"), *staticMesh->GetStaticMesh()->GetBounds().GetBox().Max.ToString());

	sphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	sphereComponent->SetGenerateOverlapEvents(true);
	//setting a collision profile; each profile has default behaviors defined per channel
	sphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	//setting collision response type
	sphereComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereComponent->SetupAttachment(root, FName("SphereComponent"));
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Miscellaneous.h"

AItem_Miscellaneous::AItem_Miscellaneous(const FObjectInitializer& OI) : Super(OI)
{
	//??
	//Super::AItem();

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	staticMesh->SetStaticMesh(SphereMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	staticMesh->SetMaterial(0, (UMaterial*)Material.Object);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMesh->SetupAttachment(root, FName("StaticMesh"));

	focusWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, FVector(staticMesh->GetStaticMesh()->GetBounds().GetBox().GetSize()).Z * 1.5f));
	UE_LOG(LogTemp, Warning, TEXT("ITEM_MISC getBox getSize: %s"), *staticMesh->GetStaticMesh()->GetBounds().GetBox().GetSize().ToString());
	UE_LOG(LogTemp, Warning, TEXT("ITEM_MISC getBox max: %s"), *staticMesh->GetStaticMesh()->GetBounds().GetBox().Max.ToString());

	sphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	sphereComponent->SetGenerateOverlapEvents(true);
	//setting a collision profile; each profile has default behaviors defined per channel
	sphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	//setting collision response type
	sphereComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereComponent->SetupAttachment(root, FName("SphereComponent"));
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
}