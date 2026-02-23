<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IngameTime.h"
#include "WaynetHandler.h"
#include "QuestHandler.h"
#include "NPCDataHandler.h"
#include "AICharacter.h"
#include "NPCController.h"
#include "Math/TransformNonVectorized.h"

#include "RPGGameInstance.generated.h"


UCLASS()
class RPG_CPP_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	uint64 global_time;
	IngameTime ingame_time;
	WaynetHandler waynet_handler;
	QuestHandler quest_handler;
	NPCDataHandler npc_data_handler;

	UPROPERTY()
	FTimerHandle global_timerHandle;

	UPROPERTY()
	FTimerHandle ingame_timerHandle;

	UFUNCTION()
	void spawnNPCs();

	UFUNCTION()
	void incGlobalSeconds();

	UFUNCTION()
	void debugPrintTime();

public:
	virtual void OnStart() override;
	
	IngameTime& getIngameTime();
	WaynetHandler& getWaynetHandler();
	NPCDataHandler& getNpcDataHandler();
	QuestHandler& getQuestHandler();
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IngameTime.h"
#include "WaynetHandler.h"
#include "QuestHandler.h"
#include "NPCDataHandler.h"
#include "AICharacter.h"
#include "NPCController.h"
#include "Math/TransformNonVectorized.h"

#include "RPGGameInstance.generated.h"


UCLASS()
class RPG_CPP_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	uint64 global_time;
	IngameTime ingame_time;
	WaynetHandler waynet_handler;
	QuestHandler quest_handler;
	NPCDataHandler npc_data_handler;

	UPROPERTY()
	FTimerHandle global_timerHandle;

	UPROPERTY()
	FTimerHandle ingame_timerHandle;

	UFUNCTION()
	void spawnNPCs();

	UFUNCTION()
	void incGlobalSeconds();

	UFUNCTION()
	void debugPrintTime();

public:
	virtual void OnStart() override;
	
	IngameTime& getIngameTime();
	WaynetHandler& getWaynetHandler();
	NPCDataHandler& getNpcDataHandler();
	QuestHandler& getQuestHandler();
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
