<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameTime.h"

// gets called every reallife second
void IngameTime::updateIngameTime()
{
	if( second + 24 >= 60 )
	{
		if( minute + 1 >= 60 )
			hour = (hour + 1) % 24;
		minute = (minute + 1) % 60;
	}	
	second = (second + 24) % 60;
}

IngameTime::IngameTime()
{
}

IngameTime::~IngameTime()
{
}
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameTime.h"

// gets called every reallife second
void IngameTime::updateIngameTime()
{
	if( second + 24 >= 60 )
	{
		if( minute + 1 >= 60 )
			hour = (hour + 1) % 24;
		minute = (minute + 1) % 60;
	}	
	second = (second + 24) % 60;
}

IngameTime::IngameTime()
{
}

IngameTime::~IngameTime()
{
}
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
