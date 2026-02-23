<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSubpart.h"

/**
 * 
 */
class RPG_CPP_API Quest
{
	bool permanent;
	bool ambient;
	bool initiative;

	TArray<QuestSubpart> subparts;
	QuestSubpart current_subpart;
	uint32 subpart_index;

	FString identifier;
	uint32 id;

	enum QS_STATUS
	{
		QS_STATUS_INACTIVE = 0,
		QS_STATUC_ACTIVE = 1,
		QS_STATUS_COMPLETED = 2,
		QS_STATUS_FAILED = 3,
	};

	QS_STATUS status;

public:
	Quest();
	~Quest();

	Quest(bool qPermanent, bool qAmbient, bool bInitiative, TArray<QuestSubpart>& qSubparts);
	
	void completeSubpart();
	void completeQuest();

	// maybe
	// TArray questRewardss
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSubpart.h"

/**
 * 
 */
class RPG_CPP_API Quest
{
	bool permanent;
	bool ambient;
	bool initiative;

	TArray<QuestSubpart> subparts;
	QuestSubpart current_subpart;
	uint32 subpart_index;

	FString identifier;
	uint32 id;

	enum QS_STATUS
	{
		QS_STATUS_INACTIVE = 0,
		QS_STATUC_ACTIVE = 1,
		QS_STATUS_COMPLETED = 2,
		QS_STATUS_FAILED = 3,
	};

	QS_STATUS status;

public:
	Quest();
	~Quest();

	Quest(bool qPermanent, bool qAmbient, bool bInitiative, TArray<QuestSubpart>& qSubparts);
	
	void completeSubpart();
	void completeQuest();

	// maybe
	// TArray questRewardss
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
