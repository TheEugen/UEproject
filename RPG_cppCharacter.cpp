<<<<<<< HEAD
// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_cppCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AICharacter.h"


////////////////////////////////

UFUNCTION()
void ARPG_cppCharacter::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if( this == OtherActor )
		return;
	
	FStringClassReference focusWidgetClassRef(TEXT("/Game/ThirdPersonCPP/UI/Focus/FocusWidget.FocusWidget_C"));
	if ( UClass* focusWidgetClass = focusWidgetClassRef.TryLoadClass<UUserWidget>() )
	{
		// create the widget
		ARPGPlayerController* playerController = (ARPGPlayerController *)UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);
		focusWidget = (UFocusWidget*) CreateWidget<UUserWidget>(playerController, focusWidgetClass, FName("FocusWidget"));
		
		UTextBlock* textWidget = (UTextBlock*) focusWidget->WidgetTree->RootWidget;

		// otherActor is npc
		if( AAICharacter* npc = Cast<AAICharacter>(OtherActor) )
		{	
			textWidget->SetText(FText::FromString(npc->getName()));
			npc->focusWidgetComponent->SetWidget(focusWidget);
			npc->focusWidgetComponent->SetVisibility(true);
		}
		// otherActor is container
		else if( AContainer* container = Cast<AContainer>(OtherActor) )
		{		
			textWidget->SetText(FText::FromString(container->name));
			container->focusWidgetComponent->SetWidget(focusWidget);
			container->focusWidgetComponent->SetVisibility(true);
		}
		// otherActor is item
		else if( AItem* item = Cast<AItem>(OtherActor) )
		{		
			textWidget->SetText(FText::FromString(item->itemStruct.name));
			item->focusWidgetComponent->SetWidget(focusWidget);
			item->focusWidgetComponent->SetVisibility(true);
		}
	}
}

UFUNCTION()
void ARPG_cppCharacter::onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if( focusWidget )
	{
		// otherActor is npc
		if( AAICharacter* npc = Cast<AAICharacter>(OtherActor) )
		{	
			npc->focusWidgetComponent->SetVisibility(false);
		}
		// otherActor is container
		else if( AContainer* container = Cast<AContainer>(OtherActor) )
		{		
			container->focusWidgetComponent->SetVisibility(false);		}
		// otherActor is item
		else if( AItem* item = Cast<AItem>(OtherActor) )
		{		
			item->focusWidgetComponent->SetVisibility(false);
		}

		focusWidget->RemoveFromParent();
		focusWidget = nullptr;
	}
}

void ARPG_cppCharacter::Inventory()
{
	inventoryComponent->toggleInventory(FAnchors(0.6f, 0.2f, 1.f, 0.8f));
}

void ARPG_cppCharacter::Interact()
{
	TArray<AActor*> overlappingActors;
	TSubclassOf<AItem> itemSubclass;
	GetOverlappingActors(overlappingActors, itemSubclass);
	for(int i = 0; i < overlappingActors.Num(); ++i)
	{
		if( overlappingActors[i]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) )
		{
			interactTarget = overlappingActors[i];
			IInteractInterface* iInterface = Cast<IInteractInterface>(overlappingActors[i]);
			iInterface->Execute_interact(Cast<UObject>(iInterface), this);
			break;
		}
	}
}

/*
UFUNCTION()
void ARPG_cppCharacter::printTime()
{
	FString time = FString();
	if( ingameTimeObj.hour < 10 )
		time += "0";
	time += FString::FromInt(ingameTimeObj.hour);
	time += ":";
	if( ingameTimeObj.minute < 10 )
		time += "0";
	time += FString::FromInt(ingameTimeObj.minute);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Yellow, time);
}

UFUNCTION()
void ARPG_cppCharacter::incGlobalSeconds()
{
	++elapsedSeconds;

	ingameTimeObj.updateIngameTime();

	FString elapsedSecondsStr = FString::FromInt((elapsedSeconds/60)%60) + ":" + FString::FromInt((elapsedSeconds/10)%6) + FString::FromInt(elapsedSeconds%10);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Green, elapsedSecondsStr);
}
*/

void ARPG_cppCharacter::BeginPlay()
{
	Super::BeginPlay();

	focusField->OnComponentBeginOverlap.AddDynamic(this, &ARPG_cppCharacter::onBeginOverlap);
	focusField->OnComponentEndOverlap.AddDynamic(this, &ARPG_cppCharacter::onEndOverlap);

	//GetWorldTimerManager().SetTimer(globalTimerHandle, this, &ARPG_cppCharacter::incGlobalSeconds, 1.f, true, 1.f);
	//GetWorldTimerManager().SetTimer(ingameTimerHandle, this, &ARPG_cppCharacter::printTime, 2.5f, true, 1.f);
}

//////////////////////////////////////////////////////////////////////////
// ARPG_cppCharacter

ARPG_cppCharacter::ARPG_cppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	AddOwnedComponent(inventoryComponent);

	FID = 0;

	focusField = CreateDefaultSubobject<UBoxComponent>(FName("FocusField"));
	focusField->SetCollisionProfileName(TEXT("Trigger"));
	focusField->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_cppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPG_cppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPG_cppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPG_cppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPG_cppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPG_cppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPG_cppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPG_cppCharacter::OnResetVR);

	// Inventory
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARPG_cppCharacter::Inventory);
	PlayerInputComponent->BindAction("Inventory", IE_Repeat, this, &ARPG_cppCharacter::Inventory);

	// Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPG_cppCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Repeat, this, &ARPG_cppCharacter::Interact);
}


void ARPG_cppCharacter::OnResetVR()
{
	// If RPG_cpp is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in RPG_cpp.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPG_cppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARPG_cppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARPG_cppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_cppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_cppCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPG_cppCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
=======
// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_cppCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AICharacter.h"


////////////////////////////////

UFUNCTION()
void ARPG_cppCharacter::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if( this == OtherActor )
		return;
	
	FStringClassReference focusWidgetClassRef(TEXT("/Game/ThirdPersonCPP/UI/Focus/FocusWidget.FocusWidget_C"));
	if ( UClass* focusWidgetClass = focusWidgetClassRef.TryLoadClass<UUserWidget>() )
	{
		// create the widget
		ARPGPlayerController* playerController = (ARPGPlayerController *)UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);
		focusWidget = (UFocusWidget*) CreateWidget<UUserWidget>(playerController, focusWidgetClass, FName("FocusWidget"));
		
		UTextBlock* textWidget = (UTextBlock*) focusWidget->WidgetTree->RootWidget;

		// otherActor is npc
		if( AAICharacter* npc = Cast<AAICharacter>(OtherActor) )
		{	
			textWidget->SetText(FText::FromString(npc->getName()));
			npc->focusWidgetComponent->SetWidget(focusWidget);
			npc->focusWidgetComponent->SetVisibility(true);
		}
		// otherActor is container
		else if( AContainer* container = Cast<AContainer>(OtherActor) )
		{		
			textWidget->SetText(FText::FromString(container->name));
			container->focusWidgetComponent->SetWidget(focusWidget);
			container->focusWidgetComponent->SetVisibility(true);
		}
		// otherActor is item
		else if( AItem* item = Cast<AItem>(OtherActor) )
		{		
			textWidget->SetText(FText::FromString(item->itemStruct.name));
			item->focusWidgetComponent->SetWidget(focusWidget);
			item->focusWidgetComponent->SetVisibility(true);
		}
	}
}

UFUNCTION()
void ARPG_cppCharacter::onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if( focusWidget )
	{
		// otherActor is npc
		if( AAICharacter* npc = Cast<AAICharacter>(OtherActor) )
		{	
			npc->focusWidgetComponent->SetVisibility(false);
		}
		// otherActor is container
		else if( AContainer* container = Cast<AContainer>(OtherActor) )
		{		
			container->focusWidgetComponent->SetVisibility(false);		}
		// otherActor is item
		else if( AItem* item = Cast<AItem>(OtherActor) )
		{		
			item->focusWidgetComponent->SetVisibility(false);
		}

		focusWidget->RemoveFromParent();
		focusWidget = nullptr;
	}
}

void ARPG_cppCharacter::Inventory()
{
	inventoryComponent->toggleInventory(FAnchors(0.6f, 0.2f, 1.f, 0.8f));
}

void ARPG_cppCharacter::Interact()
{
	TArray<AActor*> overlappingActors;
	TSubclassOf<AItem> itemSubclass;
	GetOverlappingActors(overlappingActors, itemSubclass);
	for(int i = 0; i < overlappingActors.Num(); ++i)
	{
		if( overlappingActors[i]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) )
		{
			interactTarget = overlappingActors[i];
			IInteractInterface* iInterface = Cast<IInteractInterface>(overlappingActors[i]);
			iInterface->Execute_interact(Cast<UObject>(iInterface), this);
			break;
		}
	}
}

/*
UFUNCTION()
void ARPG_cppCharacter::printTime()
{
	FString time = FString();
	if( ingameTimeObj.hour < 10 )
		time += "0";
	time += FString::FromInt(ingameTimeObj.hour);
	time += ":";
	if( ingameTimeObj.minute < 10 )
		time += "0";
	time += FString::FromInt(ingameTimeObj.minute);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Yellow, time);
}

UFUNCTION()
void ARPG_cppCharacter::incGlobalSeconds()
{
	++elapsedSeconds;

	ingameTimeObj.updateIngameTime();

	FString elapsedSecondsStr = FString::FromInt((elapsedSeconds/60)%60) + ":" + FString::FromInt((elapsedSeconds/10)%6) + FString::FromInt(elapsedSeconds%10);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Green, elapsedSecondsStr);
}
*/

void ARPG_cppCharacter::BeginPlay()
{
	Super::BeginPlay();

	focusField->OnComponentBeginOverlap.AddDynamic(this, &ARPG_cppCharacter::onBeginOverlap);
	focusField->OnComponentEndOverlap.AddDynamic(this, &ARPG_cppCharacter::onEndOverlap);

	//GetWorldTimerManager().SetTimer(globalTimerHandle, this, &ARPG_cppCharacter::incGlobalSeconds, 1.f, true, 1.f);
	//GetWorldTimerManager().SetTimer(ingameTimerHandle, this, &ARPG_cppCharacter::printTime, 2.5f, true, 1.f);
}

//////////////////////////////////////////////////////////////////////////
// ARPG_cppCharacter

ARPG_cppCharacter::ARPG_cppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	AddOwnedComponent(inventoryComponent);

	FID = 0;

	focusField = CreateDefaultSubobject<UBoxComponent>(FName("FocusField"));
	focusField->SetCollisionProfileName(TEXT("Trigger"));
	focusField->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_cppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPG_cppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPG_cppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPG_cppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPG_cppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPG_cppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPG_cppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPG_cppCharacter::OnResetVR);

	// Inventory
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARPG_cppCharacter::Inventory);
	PlayerInputComponent->BindAction("Inventory", IE_Repeat, this, &ARPG_cppCharacter::Inventory);

	// Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPG_cppCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Repeat, this, &ARPG_cppCharacter::Interact);
}


void ARPG_cppCharacter::OnResetVR()
{
	// If RPG_cpp is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in RPG_cpp.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPG_cppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARPG_cppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARPG_cppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_cppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_cppCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPG_cppCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
