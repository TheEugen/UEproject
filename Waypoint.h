// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

#include "Waypoint.generated.h"


UCLASS()
class RPG_CPP_API AWaypoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* sphereComponent;

	UPROPERTY(EditAnywhere)
	FString name;

	UPROPERTY(EditAnywhere)
	TArray<AWaypoint*> nextNodes;

	static uint32 objCount;

public:
	AWaypoint();

	FString getName();

	// idea: pass current node into function and return list without it
	TArray<AWaypoint*> getNextNodes();

	UFUNCTION()
	static AWaypoint* findWaypointByName(TArray<AWaypoint*>& waynet, FString waypointName);
	

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
