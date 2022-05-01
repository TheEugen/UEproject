// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/WidgetTree.h"
#include "AIController.h"
#include "WaynetHandler.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "DailyRoutine.h"
#include "RPGPlayerController.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "NPCController.generated.h"

//forward
class URPGGameInstance;
class Dialog;
class DialogInstruction;


UCLASS()
class RPG_CPP_API ANPCController : public AAIController
{
	GENERATED_BODY()

    UPROPERTY()
    TArray<AWaypoint*> currentPath;

    bool bIsMoving = false;
    bool bIsBusy = false;

    DailyRoutine currentDailyRoutine;

    DailyRoutine lastRoutine;

    UPROPERTY()
    FTimerHandle dailyRoutine_timerHandle;

    UPROPERTY()
    URPGGameInstance* game_instance;

    UPROPERTY()
    UDialogOptionsWidget* dialogBoxWidget;

public:
    void setCurrentPath(TArray<AWaypoint*>& path);
    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
    void gotoWaypoint(FString waypointName);
    bool checkRoutines(TArray<DailyRoutine>& npc_routines, DailyRoutine& routine);
    FString getNextWaypointName();
    void interact(AActor* interactor);
    void setIsBusy(bool isBusy);

    void setDialogOptionsWidget(UDialogOptionsWidget* in_dow);

    UFUNCTION()
    void updateDailyRoutine();

    //UFUNCTION()
   // void onButtonClicked();

	virtual void BeginPlay() override;
};
