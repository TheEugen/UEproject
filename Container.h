// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"

#include "InteractInterface.h"
#include "Item.h"

#include "Container.generated.h"


// forward
class UInventoryComponent;

USTRUCT(BlueprintType)
struct FBoxItem
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UClass* item;

	UPROPERTY(EditAnywhere)
	uint32 quantity;
};

UCLASS()
class RPG_CPP_API AContainer : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USceneComponent* root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere)
	UInventoryComponent* inventoryComponent;

	UPROPERTY(EditAnywhere)
	TArray<FBoxItem> items;

public:
	void interact_Implementation(AActor* interactor) override;

public:	
	// Sets default values for this actor's properties
	AContainer();

	UPROPERTY(EditAnywhere)
	FString name = "container";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* focusWidgetComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
