// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"

// forward
class AWaypointCrossing;

uint32 AWaypoint::objCount = 0;

FString AWaypoint::getName()
{
	return name;
}

TArray<AWaypoint*> AWaypoint::getNextNodes()
{
	return nextNodes;
}

UFUNCTION()
AWaypoint* AWaypoint::findWaypointByName(TArray<AWaypoint*>& waynet, FString waypointName)
{
	for(int i = 0; i < waynet.Num(); ++i)
	{
		if( waynet[i]->name.Equals(waypointName) )
			return waynet[i];
	}

	UE_LOG(LogTemp, Fatal, TEXT("Waypoint: no waypoint found with given name"));
	return nullptr;
}

// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	staticMesh->SetStaticMesh(SphereMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	staticMesh->SetMaterial(0, (UMaterial*)Material.Object);
	staticMesh->SetHiddenInGame(true, true);
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetupAttachment(GetRootComponent(), FName("Sphere"));

	sphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	sphereComponent->SetupAttachment(GetRootComponent(), FName("Sphere"));

	name = "WP_";
	//nextNode = nullptr;
}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

	if( nextNodes.Num() > 0 )
	{
		for(AWaypoint* wp : nextNodes)
			DrawDebugLine(GetWorld(), GetActorLocation(), wp->GetActorLocation(), FColor::Red, true, -1.f, (uint8)'\000', 5.f);
	}
		
}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

