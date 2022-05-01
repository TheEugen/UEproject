// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Materials/Material.h"
#include "Components/Image.h"

#include "InventoryTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API UInventoryTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FString itemName;
	UPROPERTY(EditAnywhere)
	FString itemDescription;
	UPROPERTY(EditAnywhere)
	uint32 itemValue;
	UPROPERTY(EditAnywhere)
	uint32 itemConsume;
	UPROPERTY(EditAnywhere)
	UTexture2D* itemImage;
	UPROPERTY(EditAnywhere)
	UMaterial* renderTargetMat;
	UPROPERTY(EditAnywhere)
	UImage* uiItemImage;

protected:
	virtual void NativeConstruct() override;
};
