// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RPG_CPP_API IngameTime
{
	enum GAMETIME_DAY
	{
		GAMETIME_DAY_MONDAY = 0,
		GAMETIME_DAY_TUESDAY = 1,
		GAMETIME_DAY_WEDNESDAY = 2,
		GAMETIME_DAY_THURSDAY = 3,
		GAMETIME_DAY_FRIDAY = 4,
		GAMETIME_DAY_SATURDAY = 5,
		GAMETIME_DAY_SUNDAY = 6,
		GAMETIME_DAY_MAX = 7
	};

public:
	IngameTime();
	~IngameTime();

	// ingame start time: Monday 08:00

	GAMETIME_DAY day = GAMETIME_DAY_MONDAY;

	uint8 hour = 8;
	uint8 minute = 0;
	uint8 second = 0;

	void updateIngameTime();
};
