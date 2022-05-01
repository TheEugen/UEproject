// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RPG_CPP_API QuestSubpart
{
	bool optional;	
	TArray<bool> conditions;

public:
	QuestSubpart();
	~QuestSubpart();
};
