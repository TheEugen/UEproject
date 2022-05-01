// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDataHandler.h"

XMLHandler& NPCDataHandler::getXmlHandler()
{
	return xml_handler;
}

NPCDataHandler::NPCDataHandler()
{
	xml_handler = XMLHandler();
	xml_handler.init();
}

NPCDataHandler::~NPCDataHandler()
{
}
