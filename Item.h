// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "InteractInterface.h"
#include "Components/WidgetComponent.h"

#include "UObject/UObjectGlobals.h"

#include "Item.generated.h"

// forward
class UInventoryComponent;

USTRUCT(BlueprintType)
struct FItemStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString name = "_item";
	UPROPERTY(EditAnywhere)
	FString description = "_desc";
	UPROPERTY(EditAnywhere)
	bool isStackable = false;
	UPROPERTY(EditAnywhere)
	bool isConsumable = false;
	UPROPERTY(EditAnywhere)
	UTexture2D* thumbnail = nullptr;
	UPROPERTY(EditAnywhere)
	uint32 maxStackSize = 1;
	UPROPERTY(EditAnywhere)
	UClass* itemClass =  nullptr;
	UPROPERTY(EditAnywhere)
	uint32 consume = 0;
	UPROPERTY(EditAnywhere)
	uint32 value = 0;
	UPROPERTY(EditAnywhere)
	uint32 ownerFID = 0;
	UPROPERTY(EditAnywhere)
	UMaterial* material = nullptr;
	UPROPERTY(EditAnywhere)
	AItem* item_ref = nullptr;
};

USTRUCT()
struct FSlotStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FItemStruct itemStruct = FItemStruct();
	UPROPERTY(EditAnywhere)
	uint32 quantity = 0;
};



UCLASS()
class RPG_CPP_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	USceneComponent* root;

	UPROPERTY(EditAnywhere)
	FItemStruct itemStruct;

	UPROPERTY()
	bool isInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* focusWidgetComponent;

	// Sets default values for this actor's properties
	AItem(const FObjectInitializer& OI);

	bool useItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void interact_Implementation(AActor* interactor) override;

};

