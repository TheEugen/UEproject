// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCController.h"
#include "AIController.h"
#include "Dialog.h"
#include "DialogInstruction.h"
#include "RPGGameInstance.h"


void ANPCController::setDialogOptionsWidget(UDialogOptionsWidget* in_dow)
{
    dialogBoxWidget = in_dow;
}

void ANPCController::interact(AActor* interactor)
{
    if( dialogBoxWidget )
    {
        dialogBoxWidget->RemoveFromParent();
        dialogBoxWidget = nullptr;
        bIsBusy = false;		
        //npc_controller->setIsBusy(false);
    }
    else
    {
        FStringClassReference dialogOptionsClassRef(TEXT("/Game/ThirdPersonCPP/UI/Dialog/DialogOptionsWidget.DialogOptionsWidget_C"));
        if ( UClass* dialogOptionsClass = dialogOptionsClassRef.TryLoadClass<UUserWidget>() )
        {
            ARPGPlayerController* playerController = (ARPGPlayerController *) UGameplayStatics::GetPlayerController(GetOuter()->GetWorld(), 0);
            dialogBoxWidget = (UDialogOptionsWidget*) CreateWidget<UUserWidget>(playerController, dialogOptionsClass, FName("DialogOptions"));

            UCanvasPanel* ui_canvas = (UCanvasPanel*) playerController->pUIWidget->WidgetTree->RootWidget;
            UCanvasPanelSlot* ui_canvasSlot = ui_canvas->AddChildToCanvas(dialogBoxWidget);
            ui_canvasSlot->bAutoSize = false;
            ui_canvasSlot->SetAnchors(FAnchors(0.3, 0.7, 0.7, 0.9));

            FInputModeUIOnly inputModeUIonly = FInputModeUIOnly();
            inputModeUIonly.SetWidgetToFocus(dialogBoxWidget->TakeWidget());

            bIsBusy = true;

            TArray<UWidget*> widgets;
            dialogBoxWidget->WidgetTree->GetChildWidgets(dialogBoxWidget->WidgetTree->RootWidget, widgets);
            UVerticalBox* verticalBoxWidget = nullptr;
            for(UWidget* w : widgets)
            {
                if( w->IsA(UVerticalBox::StaticClass()) )
                {
                    verticalBoxWidget = (UVerticalBox*) w;
                    break;
                }
            }

            check(verticalBoxWidget);

            
            TArray<UDialog*> test_dialogs;
            TArray<UDialogInstruction*> instructions;
            UDialogInstruction* di = NewObject<UDialogInstruction>();
            di->setParameters(UDialogInstruction::DI::DI_PC_SAYS, (AAICharacter*)GetCharacter(), dialogBoxWidget, "Yo was geht");
            instructions.Add(di);
            UDialogInstruction* di2 = NewObject<UDialogInstruction>();
            di2->setParameters(UDialogInstruction::DI::DI_NPC_SAYS, (AAICharacter*)GetCharacter(), dialogBoxWidget, "Nicht viel");
            instructions.Add(di2);
            UDialog* dialog = NewObject<UDialog>();
            dialog->setIdentifier("TEST_DIALOG_WASGEHT");
            dialog->setInstructions(instructions);
            dialog->setTitleLine("Yo was geht");
            test_dialogs.Add(dialog);
            TArray<UDialogInstruction*> instructions2;
            UDialogInstruction* di3 = NewObject<UDialogInstruction>();
            di3->setParameters(UDialogInstruction::DI::DI_END_DIALOG, (AAICharacter*)GetCharacter(), dialogBoxWidget, "ENDE");
            instructions2.Add(di3);
            UDialog* dialog2 = NewObject<UDialog>();
            dialog2->setIdentifier("TEST_DIALOG_ENDE");
            dialog2->setInstructions(instructions2);
            dialog2->setTitleLine("ENDE");
            test_dialogs.Add(dialog2);

            TArray<FString> testLines = { "Yo was geht", "ENDE" };

            for(int i = 0; i < testLines.Num(); ++i)
            {
                FStringClassReference dialogLineClassRef(TEXT("/Game/ThirdPersonCPP/UI/Dialog/DialogLineWidget.DialogLineWidget_C"));
                if ( UClass* dialogLineClass = dialogLineClassRef.TryLoadClass<UUserWidget>() )
                {
                    UDialogLineWidget* dialogLineWidget = (UDialogLineWidget*) CreateWidget<UUserWidget>(playerController, dialogLineClass, FName("DialogLine", i));
                    dialogLineWidget->setDialog(test_dialogs[i]);
                    TArray<UWidget*> lineWidgets;
                    dialogLineWidget->WidgetTree->GetChildWidgets(dialogLineWidget->WidgetTree->RootWidget, lineWidgets);
                    UTextBlock* textWidget = nullptr;
                    for(UWidget* w : lineWidgets)
                    {
                        if( w->IsA(UTextBlock::StaticClass()) )
                        {
                            textWidget = (UTextBlock*) w;
                            break;
                        }
                    }
                    check(textWidget);
                    textWidget->SetText(FText::FromString(testLines[i]));
                    verticalBoxWidget->AddChildToVerticalBox(dialogLineWidget);
                }
            }

            // switch controls
            playerController->SetInputMode(inputModeUIonly);
        } 
    }
}

void ANPCController::setCurrentPath(TArray<AWaypoint*>& path)
{
    currentPath = TArray<AWaypoint*>(path);
}

void ANPCController::gotoWaypoint(FString waypointName)
{
    AWaypoint* wp = AWaypoint::findWaypointByName(game_instance->getWaynetHandler().getWaynet(), waypointName);
    bIsMoving = true;
    MoveToActor(wp, 5.f);
}

FString ANPCController::getNextWaypointName()
{
    TArray<AActor*> overlappingActors;
    TSubclassOf<AWaypoint> waypointClass;
    GetCharacter()->GetOverlappingActors(overlappingActors, waypointClass);
    if( overlappingActors.Num() > 0 )
        return ((AWaypoint*)overlappingActors[0])->getName();
    else
        return "WP_FOREST_START_5";
}

// if there's a routine with matching time, write it into routine
bool ANPCController::checkRoutines(TArray<DailyRoutine>& npc_routines, DailyRoutine& routine)
{
    IngameTime ingame_time = game_instance->getIngameTime();
    for(int i = 0; i < npc_routines.Num(); ++i)
    {
        if( ingame_time.hour == npc_routines[i].getTime().hour && ingame_time.minute == npc_routines[i].getTime().minute )
        {
            routine = npc_routines[i];
            return true;
        }
    }

    return false;
}

void ANPCController::setIsBusy(bool isBusy)
{
    bIsBusy = isBusy;
}

void ANPCController::BeginPlay()
{
    Super::BeginPlay();

    game_instance = (URPGGameInstance*) UGameplayStatics::GetGameInstance(GetWorld());
    game_instance->TimerManager->SetTimer(dailyRoutine_timerHandle, this, &ANPCController::updateDailyRoutine, 2.5f, true, 1.f);
}

UFUNCTION()
void ANPCController::updateDailyRoutine()
{
    // get daily routines for this npc
    XMLHandler xml_handler = game_instance->getNpcDataHandler().getXmlHandler();
    FString npc_name = ((AAICharacter*)GetCharacter())->getName();
    TArray<DailyRoutine> daily_routines = TArray<DailyRoutine>(xml_handler.getNPCDataByName(npc_name).dailyRoutines);

    // is there a new routine?
    DailyRoutine routine;
    if( checkRoutines(daily_routines, routine) ) 
    {
        lastRoutine = currentDailyRoutine;
        currentDailyRoutine = routine;

        // is the npc doing something?      
        if( !bIsBusy )
        {
            TArray<AWaypoint*> waynet = game_instance->getWaynetHandler().getWaynet();
            TArray<AWaypoint*> path;
            AWaypoint* start = nullptr;
            AWaypoint* end = nullptr;

            for(int i = 0; i < waynet.Num(); ++i)
            {
                if( waynet[i]->getName().Equals(getNextWaypointName()) )
                {
                    start = waynet[i];
                    if( end )
                        break;
                }

                if( waynet[i]->getName().Equals(currentDailyRoutine.getWaypointName()) )
                {
                    end = waynet[i];
                    if( start )
                        break;
                }                  
            }

            // TODO: pathfinding OnMoveCompleted
            path = game_instance->getWaynetHandler().findPath(start, end);
            //path = game_instance->getWaynetHandler().testGraphSearch(start, end);
            if( path.Num() > 1)
                path.RemoveAt(0);
            MoveToActor(path[0], 5.f);
            path.RemoveAt(0);
            if( path.Num() > 0 )
                setCurrentPath(path);   
        }      
    }
    // TODO: is the npc where it should be?  
}

void ANPCController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    AWaypoint* currentNode;

    if( currentPath.Num() > 0 )
    {
        currentNode = currentPath[0];
        currentPath.RemoveAt(0);
        MoveToActor(currentNode, 5.f);
    }
    else
        bIsMoving = false;
        
}