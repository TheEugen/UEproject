#include "DialogInstruction.h"
#include "NPCController.h"


UDialogInstruction::UDialogInstruction()
{
	identifier = UDialogInstruction::DI::DI_END_DIALOG;
	owner = nullptr;
	dialog_options_widget = nullptr;
}

UDialogInstruction::~UDialogInstruction()
{

}

void UDialogInstruction::setParameters(UDialogInstruction::DI id, AAICharacter* in_owner, UDialogOptionsWidget* in_dow, FString in_text)
{
	identifier = id;
	owner = in_owner;
	dialog_options_widget = in_dow;
	dialog_text = in_text;
}

void UDialogInstruction::execute()
{
	// giant function?

	check(dialog_options_widget);
	check(owner);

	FStringClassReference subtitle_widget_ClassRef(TEXT("/Game/ThirdPersonCPP/UI/Dialog/SubtitleWidget.SubtitleWidget_C"));

	switch( identifier )
	{
	case UDialogInstruction::DI::DI_PC_SAYS:	
		if ( UClass* subtitle_widget_Class = subtitle_widget_ClassRef.TryLoadClass<UUserWidget>() )
		{
			ARPGPlayerController* player_controller = (ARPGPlayerController *) UGameplayStatics::GetPlayerController(owner->GetWorld(), 0);
			USubtitleWidget* subtitle_widget = (USubtitleWidget*) CreateWidget<UUserWidget>(player_controller, subtitle_widget_Class, FName("SubtitleWidget"));
			TArray<UTextBlock*> textblocks;
			TArray<UWidget*> children;
			subtitle_widget->WidgetTree->GetChildWidgets(subtitle_widget->WidgetTree->RootWidget, children);
			for(UWidget* w : children)
			{
				if( w->IsA(UTextBlock::StaticClass()) )
					textblocks.Push((UTextBlock*) w);
			}

			// speaker name
			textblocks[0]->SetText(FText::FromString("Ich"));

			// the subtitle
			textblocks[1]->SetText(FText::FromString(dialog_text));

			

			// add widget to ui canvas
			UCanvasPanel* ui_canvas = (UCanvasPanel*) player_controller->pUIWidget->WidgetTree->RootWidget;
			UCanvasPanelSlot* ui_canvas_slot = ui_canvas->AddChildToCanvas(subtitle_widget);
			ui_canvas_slot->SetAutoSize(true);
			ui_canvas_slot->SetAnchors(FAnchors(0.25f, 0.1f, 0.75f, 0.3f));
		}
		break;

	case UDialogInstruction::DI::DI_NPC_SAYS:
		if ( UClass* subtitle_widget_Class = subtitle_widget_ClassRef.TryLoadClass<UUserWidget>() )
		{
			ARPGPlayerController* player_controller = (ARPGPlayerController *) UGameplayStatics::GetPlayerController(owner->GetWorld(), 0);
			USubtitleWidget* subtitle_widget = (USubtitleWidget*) CreateWidget<UUserWidget>(owner->GetWorld()->GetFirstPlayerController(), subtitle_widget_Class, FName("SubtitleWidget"));
			TArray<UTextBlock*> textblocks;
			TArray<UWidget*> children;
			subtitle_widget->WidgetTree->GetChildWidgets(subtitle_widget->WidgetTree->RootWidget, children);
			for(UWidget* w : children)
			{
				if( w->IsA(UTextBlock::StaticClass()) )
					textblocks.Push((UTextBlock*) w);
			}

			// speaker name
			textblocks[0]->SetText(FText::FromString(owner->getName()));

			// the subtitle
			textblocks[1]->SetText(FText::FromString(dialog_text));

			// add widget to ui canvas
			UCanvasPanel* ui_canvas = (UCanvasPanel*) player_controller->pUIWidget->WidgetTree->RootWidget;
			UCanvasPanelSlot* ui_canvas_slot = ui_canvas->AddChildToCanvas(subtitle_widget);
			ui_canvas_slot->SetAutoSize(true);
			ui_canvas_slot->SetAnchors(FAnchors(0.25f, 0.1f, 0.75f, 0.3f));
		}
		break;

	case UDialogInstruction::DI::DI_END_DIALOG:
		dialog_options_widget->RemoveFromParent();
		ANPCController* npc_controller = (ANPCController*) owner->GetController();
		npc_controller->setIsBusy(false);
		npc_controller->setDialogOptionsWidget(nullptr);
		APlayerController* player_controller = owner->GetWorld()->GetFirstPlayerController();
		player_controller->SetInputMode(FInputModeGameOnly());
		break;
	}
}