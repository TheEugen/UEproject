// Fill out your copyright notice in the Description page of Project Settings.


#include "DailyRoutine.h"


void DailyRoutine::setTime(uint32 h, uint32 m)
{
	time.hour = h;
	time.minute = m;
}

void DailyRoutine::setWaypointName(FString name)
{
	waypointName = name;
}

IngameTime DailyRoutine::getTime()
{
	return time;
}

uint32 DailyRoutine::getActivity()
{
	return activity;
}

FString DailyRoutine::getWaypointName()
{
	return waypointName;
}

DailyRoutine::DailyRoutine()
{
}

DailyRoutine::~DailyRoutine()
{
}

DailyRoutine::DailyRoutine(IngameTime time, DailyRoutineActivities activity, FString waypointName)
{
	this->time = time;
	this->activity = activity;
	this->waypointName = waypointName;
}
