// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogLineWidget.h"

#include "Dialog.h"

FString UDialogLineWidget::getText()
{
	return text;
}

void UDialogLineWidget::setText(FString in_text)
{
	text = FString(in_text);
}


void UDialogLineWidget::setDialog(UDialog* in_dialog)
{
	dialog = in_dialog;
}

UDialog* UDialogLineWidget::getDialog()
{
	return dialog;
}

UFUNCTION()
void UDialogLineWidget::onButtonClicked()
{
	dialog->start();
}

void UDialogLineWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// button is root widget
	check(WidgetTree->RootWidget->IsA(UButton::StaticClass()));

	((UButton*) WidgetTree->RootWidget)->OnClicked.RemoveDynamic(this, &UDialogLineWidget::onButtonClicked);
}

void UDialogLineWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// button is root widget
	check(WidgetTree->RootWidget->IsA(UButton::StaticClass()));

	((UButton*) WidgetTree->RootWidget)->OnClicked.AddDynamic(this, &UDialogLineWidget::onButtonClicked);
}