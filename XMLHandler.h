// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlFile.h"
#include "XmlNode.h"
#include "DailyRoutine.h"

/**
 * 
 */

struct npc_data_struct
{
	FString name;
	uint32 id;
	uint32 lvl;
	uint32 lp;
	uint32 currentHP;
	uint32 maxHP;
	uint32 currentMana;
	uint32 maxMana;
	TArray<DailyRoutine> dailyRoutines;
	FString spawnpoint_name;
};

class RPG_CPP_API XMLHandler
{
	UPROPERTY()
	FXmlFile* xmlFile;

	UPROPERTY()
	FXmlNode* xmlRootNode;

	UPROPERTY()
	TArray<npc_data_struct> npc_data;


public:
	XMLHandler();
	~XMLHandler();

	void init();
	npc_data_struct getNPCDataByName(FString npc_name);
	TArray<npc_data_struct> getNPCData();
};
