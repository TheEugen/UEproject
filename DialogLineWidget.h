// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

//#include "Dialog.h"

#include "DialogLineWidget.generated.h"

// forward
class UDialog;

UCLASS()
class RPG_CPP_API UDialogLineWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	FString text;

	UPROPERTY()
	UDialog* dialog;

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	FString getText();

	void setText(FString in_text);

	
	void setDialog(UDialog* in_dialog);

	UDialog* getDialog();

	UFUNCTION()
	void onButtonClicked();
	
};
