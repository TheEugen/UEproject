<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "DailyRoutine.h"
#include "IngameTime.h"
#include "InteractInterface.h"
#include "DialogOptionsWidget.h"
#include "Components/VerticalBox.h"
#include "DialogLineWidget.h"
#include "XMLHandler.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

#include "AICharacter.generated.h"

// forward
class ARPG_cppCharacter;
class URPGGameInstance;
class ANPCController;


UCLASS()
class RPG_CPP_API AAICharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* pawnSensingComponent;

	UPROPERTY()
	AItem* lastSeenInteractTarget = nullptr;

	UFUNCTION()
	void seePawn(APawn* pawn);

	bool bIsBusy = false;

	UPROPERTY()
	UDialogOptionsWidget* dialogBoxWidget = nullptr;

	//UPROPERTY(EditAnywhere)
	FString name = "npc";

public:
	// Sets default values for this character's properties
	AAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* focusWidgetComponent;

	UPROPERTY()
	FTimerHandle dailyRoutineTimerHandle;

	DailyRoutine currentDailyRoutine;

	void setName(FString npcName);
	FString getName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void interact_Implementation(AActor* interactor) override;
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "DailyRoutine.h"
#include "IngameTime.h"
#include "InteractInterface.h"
#include "DialogOptionsWidget.h"
#include "Components/VerticalBox.h"
#include "DialogLineWidget.h"
#include "XMLHandler.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

#include "AICharacter.generated.h"

// forward
class ARPG_cppCharacter;
class URPGGameInstance;
class ANPCController;


UCLASS()
class RPG_CPP_API AAICharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* pawnSensingComponent;

	UPROPERTY()
	AItem* lastSeenInteractTarget = nullptr;

	UFUNCTION()
	void seePawn(APawn* pawn);

	bool bIsBusy = false;

	UPROPERTY()
	UDialogOptionsWidget* dialogBoxWidget = nullptr;

	//UPROPERTY(EditAnywhere)
	FString name = "npc";

public:
	// Sets default values for this character's properties
	AAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* focusWidgetComponent;

	UPROPERTY()
	FTimerHandle dailyRoutineTimerHandle;

	DailyRoutine currentDailyRoutine;

	void setName(FString npcName);
	FString getName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void interact_Implementation(AActor* interactor) override;
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
