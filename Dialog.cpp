#include "Dialog.h"

FString UDialog::getTitleLine()
{
	return title_line;
}

void UDialog::setIdentifier(FString id)
{
	identifier = FString(id);
}

void UDialog::setInstructions(TArray<UDialogInstruction*>& in_instructions)
{
	instructions = TArray<UDialogInstruction*>(in_instructions);
}

void UDialog::setTitleLine(FString line)
{
	title_line = FString(line);
}

FString UDialog::getIdentifier()
{
	return identifier;
}

TArray<UDialogInstruction*>& UDialog::getInstructions()
{
	return instructions;
}

void UDialog::start()
{
	for(UDialogInstruction* di : instructions)
		di->execute();
}

UDialog::UDialog(FString id, TArray<UDialogInstruction*>& in_instructions, FString in_title_line)
{
	identifier = FString(id);
	instructions = TArray<UDialogInstruction*>(in_instructions);
	title_line = in_title_line;
}

UDialog::UDialog()
{
}

UDialog::~UDialog()
{
}