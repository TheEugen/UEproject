// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Waypoint.h"
#include "Containers/Queue.h"

/**
 * 
 */
class RPG_CPP_API WaynetHandler
{
	TArray<AWaypoint*> waynet;

public:
	WaynetHandler();
	~WaynetHandler();

	WaynetHandler(TArray<AActor*>& actor_waynet);

	TArray<AWaypoint*> findPath(AWaypoint* start, AWaypoint* end);
	TArray<AWaypoint*>& getWaynet();

	TArray<AWaypoint*> testGraphSearch(AWaypoint* start, AWaypoint* end);
};
