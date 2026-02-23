<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UUserWidget> UIWidget;
	UPROPERTY(VisibleAnywhere)
	UUserWidget* pUIWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UUserWidget> UIWidget;
	UPROPERTY(VisibleAnywhere)
	UUserWidget* pUIWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
