<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SubtitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API USubtitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;
};
=======
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SubtitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_CPP_API USubtitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;
};
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
