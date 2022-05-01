// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"

#include "Item_Miscellaneous.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API AItem_Miscellaneous : public AItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* sphereComponent;

public:
	AItem_Miscellaneous(const FObjectInitializer& OI);
	
};
