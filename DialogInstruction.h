<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AICharacter.h"
#include "DialogOptionsWidget.h"
#include "SubtitleWidget.h"

#include "DialogInstruction.generated.h"


// forward
class ANPCController;

UCLASS()
class RPG_CPP_API UDialogInstruction : public UObject
{
	GENERATED_BODY()

public:	
	enum DI
	{
		DI_PC_SAYS = 0,
		DI_NPC_SAYS = 1,
		DI_PLAY_ANIM = 2,
		DI_PLAY_SOUND = 3,		// dialog should continue with next instruction while playing
		DI_DO_ACTION = 4,		// composition of anims? e.g. npc walks to a nearby firecamp, takes some item, and comes back
		DI_ATTACK_TARGET = 5,
		DI_GIVE_ITEM = 6,		// pc to npc and vice versa
		DI_UPDATE_QUEST = 7,
		DI_SPAWN_NPC = 8,
		DI_SPAWN_MONSTER = 9,
		DI_END_DIALOG = 9999
	};

	UDialogInstruction();

	~UDialogInstruction();

	void setParameters(UDialogInstruction::DI id, AAICharacter* in_owner, UDialogOptionsWidget* in_dow, FString in_text);

	// virtual?
	void execute();

private:
	// id
	UDialogInstruction::DI identifier;

	// npc who owns this dialog
	UPROPERTY()
	AAICharacter* owner;

	// use event instead of ref?
	// dialog options widget ref
	UPROPERTY()
	UDialogOptionsWidget* dialog_options_widget;

	FString dialog_text;
	
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AICharacter.h"
#include "DialogOptionsWidget.h"
#include "SubtitleWidget.h"

#include "DialogInstruction.generated.h"


// forward
class ANPCController;

UCLASS()
class RPG_CPP_API UDialogInstruction : public UObject
{
	GENERATED_BODY()

public:	
	enum DI
	{
		DI_PC_SAYS = 0,
		DI_NPC_SAYS = 1,
		DI_PLAY_ANIM = 2,
		DI_PLAY_SOUND = 3,		// dialog should continue with next instruction while playing
		DI_DO_ACTION = 4,		// composition of anims? e.g. npc walks to a nearby firecamp, takes some item, and comes back
		DI_ATTACK_TARGET = 5,
		DI_GIVE_ITEM = 6,		// pc to npc and vice versa
		DI_UPDATE_QUEST = 7,
		DI_SPAWN_NPC = 8,
		DI_SPAWN_MONSTER = 9,
		DI_END_DIALOG = 9999
	};

	UDialogInstruction();

	~UDialogInstruction();

	void setParameters(UDialogInstruction::DI id, AAICharacter* in_owner, UDialogOptionsWidget* in_dow, FString in_text);

	// virtual?
	void execute();

private:
	// id
	UDialogInstruction::DI identifier;

	// npc who owns this dialog
	UPROPERTY()
	AAICharacter* owner;

	// use event instead of ref?
	// dialog options widget ref
	UPROPERTY()
	UDialogOptionsWidget* dialog_options_widget;

	FString dialog_text;
	
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
