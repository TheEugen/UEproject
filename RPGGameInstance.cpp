// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"

UFUNCTION()
void URPGGameInstance::incGlobalSeconds()
{
	ingame_time.updateIngameTime();
	++global_time;
	FString global_timeStr = FString::FromInt((global_time/60)%60) + ":" + FString::FromInt((global_time/10)%6) + FString::FromInt(global_time%10);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Green, global_timeStr);
	

	/*
	if( ingame_time.hour == 8 && ingame_time.minute == 2 )
	{
		TArray<AWaypoint*> test_path;
		test_path = waynet_handler.findPath(AWaypoint::findWaypointByName(waynet_handler.getWaynet(), "WP_14"),
			AWaypoint::findWaypointByName(waynet_handler.getWaynet(), "WP_27"));

		TArray<AWaypoint*> test_path_new;
		test_path_new = waynet_handler.testGraphSearch(AWaypoint::findWaypointByName(waynet_handler.getWaynet(), "WP_27"),
														AWaypoint::findWaypointByName(waynet_handler.getWaynet(), "WP_14"));

		for(AWaypoint* wp : test_path)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Blue, wp->getName());

		for(AWaypoint* wp : test_path_new)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Red, wp->getName());
	}
	*/
}

UFUNCTION()
void URPGGameInstance::debugPrintTime()
{
	FString time = FString();
	if( ingame_time.hour < 10 )
		time += "0";
	time += FString::FromInt(ingame_time.hour);
	time += ":";
	if( ingame_time.minute < 10 )
		time += "0";
	time += FString::FromInt(ingame_time.minute);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100.f, FColor::Yellow, time);
}

UFUNCTION()
void URPGGameInstance::spawnNPCs()
{
	FRotator rot = FRotator(0.f);
	FActorSpawnParameters spawnParameters = FActorSpawnParameters();
	spawnParameters.bNoFail = true;

	// get data tables of all npcs
	TArray<npc_data_struct> npcList = npc_data_handler.getXmlHandler().getNPCData();

	bool pcSeen = false;
	for(int i = 0; i < npcList.Num(); ++i)
	{
		if( !pcSeen )
		{
			pcSeen = true;
			continue;
		}

		FVector loc = AWaypoint::findWaypointByName(waynet_handler.getWaynet(), npcList[i].spawnpoint_name)->GetActorLocation();
		AAICharacter* character = (AAICharacter*) GetWorld()->SpawnActor(AAICharacter::StaticClass(), &loc, &rot, spawnParameters);
		check(character);
		character->setName(npcList[i].name);
		character->AIControllerClass = ANPCController::StaticClass();
		character->SpawnDefaultController();
	}
}

NPCDataHandler& URPGGameInstance::getNpcDataHandler()
{
	return npc_data_handler;
}

WaynetHandler& URPGGameInstance::getWaynetHandler()
{
	return waynet_handler;
}

IngameTime& URPGGameInstance::getIngameTime()
{
	return ingame_time;
}

void URPGGameInstance::OnStart()
{
	//Super::OnStart();

	// time
	TimerManager->SetTimer(global_timerHandle, this, &URPGGameInstance::incGlobalSeconds, 1.f, true, 1.f);
	TimerManager->SetTimer(ingame_timerHandle, this, &URPGGameInstance::debugPrintTime, 2.5f, true, 1.f);

	// waynet
	TArray<AActor*> waynet;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), waynet);
	waynet_handler = WaynetHandler(waynet);

	// npc data
	npc_data_handler = NPCDataHandler();

	// quests
	quest_handler = QuestHandler();

	// spawning delegates
	GetWorld()->OnWorldBeginPlay.AddUFunction(this, FName("spawnNPCs"));

}