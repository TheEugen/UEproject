// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"


void Quest::completeSubpart()
{
	++subpart_index;

	current_subpart = subparts[subpart_index];	

	if( subpart_index == subparts.Num() - 1 )
		completeQuest();
}

void Quest::completeQuest()
{
	status = QS_STATUS_COMPLETED;
}

Quest::Quest(bool qPermanent, bool qAmbient, bool qInitiative, TArray<QuestSubpart>& qSubparts)
{
	permanent = qPermanent;
	ambient = qAmbient;
	initiative = qInitiative;

	check(qSubparts.Num() > 0);
	subparts = TArray<QuestSubpart>(qSubparts);
	current_subpart = subparts[0];

	subpart_index = 0;
	status = QS_STATUS_INACTIVE;
}

Quest::Quest()
{
	permanent = false;
	ambient = false;
	initiative = false;

	subpart_index = 0;

	status = QS_STATUS_INACTIVE;
}

Quest::~Quest()
{
}
