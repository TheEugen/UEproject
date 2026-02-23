<<<<<<< HEAD
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
=======
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
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
