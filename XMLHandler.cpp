<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "XMLHandler.h"


TArray<npc_data_struct> XMLHandler::getNPCData()
{
	return npc_data;
}

npc_data_struct XMLHandler::getNPCDataByName(FString npc_name)
{
	check(xmlRootNode);

	npc_data_struct npc;

	for(npc_data_struct s : npc_data)
	{
		if( s.name.Equals(npc_name) )
			return s;
	}

	UE_LOG(LogTemp, Fatal, TEXT("XMLHandler: no npc_data_struct with given name found"));
	return npc;
}

void XMLHandler::init()
{
	check(xmlRootNode);
	TArray<FXmlNode*> npcNodes = xmlRootNode->GetChildrenNodes();
	for(FXmlNode* node : npcNodes)
	{
		npc_data_struct npc_struct;

		TArray<FXmlNode*> nodes;
		nodes = node->GetChildrenNodes();

		// nodes[0] attributes nodes [1] spawnpoint [2] daily routines
		TArray<FXmlNode*> attributeNodes;
		attributeNodes = nodes[0]->GetChildrenNodes();

		// Attributes	
		npc_struct.name = FString(attributeNodes[0]->GetContent().GetCharArray().GetData());
		npc_struct.id = FCString::Atoi(attributeNodes[1]->GetContent().GetCharArray().GetData());
		npc_struct.lvl = FCString::Atoi(attributeNodes[2]->GetContent().GetCharArray().GetData());
		npc_struct.lp = FCString::Atoi(attributeNodes[3]->GetContent().GetCharArray().GetData());
		npc_struct.currentHP = FCString::Atoi(attributeNodes[4]->GetContent().GetCharArray().GetData());
		npc_struct.maxHP = FCString::Atoi(attributeNodes[5]->GetContent().GetCharArray().GetData());
		npc_struct.currentMana = FCString::Atoi(attributeNodes[6]->GetContent().GetCharArray().GetData());
		npc_struct.maxMana = FCString::Atoi(attributeNodes[7]->GetContent().GetCharArray().GetData());
		
		// Spawnpoints
		npc_struct.spawnpoint_name = FString(nodes[1]->GetContent().GetCharArray().GetData());

		// Daily Routines
		for(FXmlNode* node_routine : nodes[2]->GetChildrenNodes())
		{
			TArray<FXmlNode*> routineNodes = node_routine->GetChildrenNodes();
			uint32 h = FCString::Atoi(routineNodes[0]->GetContent().GetCharArray().GetData());
			uint32 m = FCString::Atoi(routineNodes[1]->GetContent().GetCharArray().GetData());
			FString waypointName = FString(routineNodes[2]->GetContent().GetCharArray().GetData());
			IngameTime time = IngameTime();
			time.hour = h;
			time.minute = m;
			DailyRoutine tmp_dailyRoutine = DailyRoutine(time, DailyRoutine::DR_STAND, waypointName);
			npc_struct.dailyRoutines.Add(tmp_dailyRoutine);
		}

		npc_data.Add(npc_struct);

	}
	
}

XMLHandler::XMLHandler()
{
	xmlFile = new FXmlFile("C:\\dev\\Unreal Projects\\RPG_cpp 5.0\\Source\\RPG_cpp\\Data\\DATA_NPC.xml", EConstructMethod::ConstructFromFile);
	xmlRootNode = xmlFile->GetRootNode();
}

XMLHandler::~XMLHandler()
{
}
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "XMLHandler.h"


TArray<npc_data_struct> XMLHandler::getNPCData()
{
	return npc_data;
}

npc_data_struct XMLHandler::getNPCDataByName(FString npc_name)
{
	check(xmlRootNode);

	npc_data_struct npc;

	for(npc_data_struct s : npc_data)
	{
		if( s.name.Equals(npc_name) )
			return s;
	}

	UE_LOG(LogTemp, Fatal, TEXT("XMLHandler: no npc_data_struct with given name found"));
	return npc;
}

void XMLHandler::init()
{
	check(xmlRootNode);
	TArray<FXmlNode*> npcNodes = xmlRootNode->GetChildrenNodes();
	for(FXmlNode* node : npcNodes)
	{
		npc_data_struct npc_struct;

		TArray<FXmlNode*> nodes;
		nodes = node->GetChildrenNodes();

		// nodes[0] attributes nodes [1] spawnpoint [2] daily routines
		TArray<FXmlNode*> attributeNodes;
		attributeNodes = nodes[0]->GetChildrenNodes();

		// Attributes	
		npc_struct.name = FString(attributeNodes[0]->GetContent().GetCharArray().GetData());
		npc_struct.id = FCString::Atoi(attributeNodes[1]->GetContent().GetCharArray().GetData());
		npc_struct.lvl = FCString::Atoi(attributeNodes[2]->GetContent().GetCharArray().GetData());
		npc_struct.lp = FCString::Atoi(attributeNodes[3]->GetContent().GetCharArray().GetData());
		npc_struct.currentHP = FCString::Atoi(attributeNodes[4]->GetContent().GetCharArray().GetData());
		npc_struct.maxHP = FCString::Atoi(attributeNodes[5]->GetContent().GetCharArray().GetData());
		npc_struct.currentMana = FCString::Atoi(attributeNodes[6]->GetContent().GetCharArray().GetData());
		npc_struct.maxMana = FCString::Atoi(attributeNodes[7]->GetContent().GetCharArray().GetData());
		
		// Spawnpoints
		npc_struct.spawnpoint_name = FString(nodes[1]->GetContent().GetCharArray().GetData());

		// Daily Routines
		for(FXmlNode* node_routine : nodes[2]->GetChildrenNodes())
		{
			TArray<FXmlNode*> routineNodes = node_routine->GetChildrenNodes();
			uint32 h = FCString::Atoi(routineNodes[0]->GetContent().GetCharArray().GetData());
			uint32 m = FCString::Atoi(routineNodes[1]->GetContent().GetCharArray().GetData());
			FString waypointName = FString(routineNodes[2]->GetContent().GetCharArray().GetData());
			IngameTime time = IngameTime();
			time.hour = h;
			time.minute = m;
			DailyRoutine tmp_dailyRoutine = DailyRoutine(time, DailyRoutine::DR_STAND, waypointName);
			npc_struct.dailyRoutines.Add(tmp_dailyRoutine);
		}

		npc_data.Add(npc_struct);

	}
	
}

XMLHandler::XMLHandler()
{
	xmlFile = new FXmlFile("C:\\dev\\Unreal Projects\\RPG_cpp 5.0\\Source\\RPG_cpp\\Data\\DATA_NPC.xml", EConstructMethod::ConstructFromFile);
	xmlRootNode = xmlFile->GetRootNode();
}

XMLHandler::~XMLHandler()
{
}
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
