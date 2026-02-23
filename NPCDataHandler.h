// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XMLHandler.h"

/**
 * 
 */
class RPG_CPP_API NPCDataHandler
{
	XMLHandler xml_handler;

public:
	NPCDataHandler();
	~NPCDataHandler();

	XMLHandler& getXmlHandler();
};
