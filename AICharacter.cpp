<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "RPG_cppCharacter.h"
#include "RPGGameInstance.h"
#include "NPCController.h"


FString AAICharacter::getName()
{
	return name;
}

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	pawnSensingComponent->SetPeripheralVisionAngle(70.f);

	// focus box
	focusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FocusWidgetComponent"));
	focusWidgetComponent->SetRelativeLocation(FVector(10.f, 0.f, 105.f));
	focusWidgetComponent->SetDrawAtDesiredSize(true);
	focusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	focusWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	focusWidgetComponent->SetupAttachment(GetRootComponent(), FName("FocusWidgetComponent"));

	// assign static mesh and anim class
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	GetMesh()->SetSkeletalMesh(MeshContainer.Object);

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimContainer(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	//GetMesh()->SetAnimClass(AnimContainer.Object->GetClass());
	GetMesh()->SetAnimInstanceClass(AnimContainer.Object->GeneratedClass);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -85.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetEnableGravity(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	SetActorEnableCollision(true);
}

void AAICharacter::interact_Implementation(AActor* interactor)
{
	ANPCController* npc_controller = (ANPCController*) GetController();
	npc_controller->interact(interactor);
}

UFUNCTION()
void AAICharacter::seePawn(APawn* pawn)
{
	// react to theft

	// pawn npc sees is pc
	if( ARPG_cppCharacter* character = (ARPG_cppCharacter*) pawn)
	{
		// get items that overlap with pc
		TArray<AActor*> overlappingActors;
		TSubclassOf<AItem> actorClass;
		character->GetOverlappingActors(overlappingActors, actorClass);

		// we just saw pc overlap with something
		if( lastSeenInteractTarget )
		{
			// pc took something
			if( lastSeenInteractTarget->IsPendingKill() && lastSeenInteractTarget->itemStruct.ownerFID != character->FID )
			{
				AAIController* aiController = UAIBlueprintHelperLibrary::GetAIController(this);
				aiController->MoveToActor(character, 5.f);
			}
		}
		
		// check if any item overlaps
		if( overlappingActors.Num() > 0 )
		{
			AItem* item = (AItem*) overlappingActors[0];
			lastSeenInteractTarget = item;
		}
	}
}

void AAICharacter::setName(FString npcName)
{
	name = FString::FString(npcName);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	pawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::seePawn);

	//AutoPossessAI = EAutoPossessAI::Disabled;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "RPG_cppCharacter.h"
#include "RPGGameInstance.h"
#include "NPCController.h"


FString AAICharacter::getName()
{
	return name;
}

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	pawnSensingComponent->SetPeripheralVisionAngle(70.f);

	// focus box
	focusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FocusWidgetComponent"));
	focusWidgetComponent->SetRelativeLocation(FVector(10.f, 0.f, 105.f));
	focusWidgetComponent->SetDrawAtDesiredSize(true);
	focusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	focusWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	focusWidgetComponent->SetupAttachment(GetRootComponent(), FName("FocusWidgetComponent"));

	// assign static mesh and anim class
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	GetMesh()->SetSkeletalMesh(MeshContainer.Object);

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimContainer(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	//GetMesh()->SetAnimClass(AnimContainer.Object->GetClass());
	GetMesh()->SetAnimInstanceClass(AnimContainer.Object->GeneratedClass);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -85.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetEnableGravity(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	SetActorEnableCollision(true);
}

void AAICharacter::interact_Implementation(AActor* interactor)
{
	ANPCController* npc_controller = (ANPCController*) GetController();
	npc_controller->interact(interactor);
}

UFUNCTION()
void AAICharacter::seePawn(APawn* pawn)
{
	// react to theft

	// pawn npc sees is pc
	if( ARPG_cppCharacter* character = (ARPG_cppCharacter*) pawn)
	{
		// get items that overlap with pc
		TArray<AActor*> overlappingActors;
		TSubclassOf<AItem> actorClass;
		character->GetOverlappingActors(overlappingActors, actorClass);

		// we just saw pc overlap with something
		if( lastSeenInteractTarget )
		{
			// pc took something
			if( lastSeenInteractTarget->IsPendingKill() && lastSeenInteractTarget->itemStruct.ownerFID != character->FID )
			{
				AAIController* aiController = UAIBlueprintHelperLibrary::GetAIController(this);
				aiController->MoveToActor(character, 5.f);
			}
		}
		
		// check if any item overlaps
		if( overlappingActors.Num() > 0 )
		{
			AItem* item = (AItem*) overlappingActors[0];
			lastSeenInteractTarget = item;
		}
	}
}

void AAICharacter::setName(FString npcName)
{
	name = FString::FString(npcName);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	pawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::seePawn);

	//AutoPossessAI = EAutoPossessAI::Disabled;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
