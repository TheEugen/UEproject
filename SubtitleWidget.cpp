<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget.h"

FReply USubtitleWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	RemoveFromParent();

	return FReply::Handled();
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget.h"

FReply USubtitleWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	RemoveFromParent();

	return FReply::Handled();
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
}