// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Container.h"
#include "FocusWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WaynetHandler.h"
#include "IngameTime.h"
#include "XMLHandler.h"

#include "RPG_cppCharacter.generated.h"

// forward
class AAICharacter;


UCLASS(config=Game)
class ARPG_cppCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ARPG_cppCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// inventory
	UPROPERTY(EditAnywhere)
	UInventoryComponent* inventoryComponent;

	// faction id
	UPROPERTY(EditAnywhere)
	uint32 FID;

	// focus box
	UPROPERTY(EditAnywhere)
	UBoxComponent* focusField;

	// target of interaction
	UPROPERTY()
	AActor* interactTarget = nullptr;

	// focus widget
	UPROPERTY()
	UFocusWidget* focusWidget;

	/*
	// GLOBAL timer
	UPROPERTY()
	FTimerHandle globalTimerHandle;
	
	// ingame timer
	UPROPERTY()
	FTimerHandle ingameTimerHandle;

	UPROPERTY()
	uint64 elapsedSeconds = 0;

	//UPROPERTY()
	IngameTime ingameTimeObj;

	XMLHandler xmlHandler;

	UFUNCTION()
	void printTime();

	UFUNCTION()
	void incGlobalSeconds();
	
	*/

	UFUNCTION()
	void onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Inventory();
	void Interact();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

