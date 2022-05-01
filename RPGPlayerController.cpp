// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"


void ARPGPlayerController::BeginPlay()
{
	FStringClassReference UIClassRef(TEXT("/Game/ThirdPersonCPP/UI/UI.UI_C"));
	if ( UClass* UIClass = UIClassRef.TryLoadClass<UUserWidget>() )
	{
		pUIWidget = CreateWidget<UUserWidget>(this, UIClass, FName("UI"));
		pUIWidget->bIsFocusable = true;
		pUIWidget->AddToViewport();
	}
}
		