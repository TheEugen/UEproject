// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogInstruction.h"

#include "Dialog.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API UDialog : public UObject
{
	GENERATED_BODY()
	
	// id
	UPROPERTY()
	FString identifier;

	// dialog content
	UPROPERTY()
	TArray<UDialogInstruction*> instructions;

	// line to display in widget
	UPROPERTY()
	FString title_line;

public:
	UDialog();

	UDialog(FString id, TArray<UDialogInstruction*>& in_instructions, FString in_title_line);

	~UDialog();

	void setIdentifier(FString id);
	void setInstructions(TArray<UDialogInstruction*>& in_instructions);
	void setTitleLine(FString line);

	FString getIdentifier();
	TArray<UDialogInstruction*>& getInstructions();
	FString getTitleLine();

	void start();
};
