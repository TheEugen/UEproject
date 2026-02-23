// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_cppGameMode.h"
#include "RPG_cppCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_cppGameMode::ARPG_cppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
