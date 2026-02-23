<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Waypoint.h"
#include "IngameTime.h"

/**
 * 
 */



class RPG_CPP_API DailyRoutine
{
public:
	enum DailyRoutineActivities
	{
		DR_STAND = 0,
		DR_PISS = 1,
		DR_TALK = 2,
		DR_COOK = 3,


		DR_MAX = 9999
	};

private:

	IngameTime time;
	DailyRoutineActivities activity;
	FString waypointName;

public:
	DailyRoutine();
	~DailyRoutine();

	DailyRoutine(IngameTime time, DailyRoutineActivities activity, FString waypointName);

	IngameTime getTime();
	uint32 getActivity();
	FString getWaypointName();

	void setTime(uint32 h, uint32 m);
	void setWaypointName(FString name);
	
	
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Waypoint.h"
#include "IngameTime.h"

/**
 * 
 */



class RPG_CPP_API DailyRoutine
{
public:
	enum DailyRoutineActivities
	{
		DR_STAND = 0,
		DR_PISS = 1,
		DR_TALK = 2,
		DR_COOK = 3,


		DR_MAX = 9999
	};

private:

	IngameTime time;
	DailyRoutineActivities activity;
	FString waypointName;

public:
	DailyRoutine();
	~DailyRoutine();

	DailyRoutine(IngameTime time, DailyRoutineActivities activity, FString waypointName);

	IngameTime getTime();
	uint32 getActivity();
	FString getWaypointName();

	void setTime(uint32 h, uint32 m);
	void setWaypointName(FString name);
	
	
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
