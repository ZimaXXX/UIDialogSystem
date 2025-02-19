#include "UDSDialogueManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Data/UDSDialogueDataAsset.h"
#include "Widgets/UDSDialogueWidget.h"

UUDSDialogueManagerSubsystem::UUDSDialogueManagerSubsystem()
{
}

void UUDSDialogueManagerSubsystem::StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset, UDataTable* KeywordsDataTable)
{
	if (DialogueDataAsset)
	{
		DialogueQueue = DialogueDataAsset->DialogueEntries;
		HoverKeywordsDataTable = KeywordsDataTable;
		CurrentDialogueIndex = 0;
		ShowNextDialogue();
	}
}

void UUDSDialogueManagerSubsystem::ShowNextDialogue()
{
	if (CurrentDialogueIndex < DialogueQueue.Num())
	{
		if (!CurrentDialogueWidget)
		{
			if (DialogueWidgetClass)
			{
				CurrentDialogueWidget = CreateWidget<UUDSDialogueWidget>(GetWorld(), DialogueWidgetClass);
				CurrentDialogueWidget->AddToViewport();
			}
		}

		if (CurrentDialogueWidget)
		{
			CurrentDialogueWidget->SetDialogue(DialogueQueue[CurrentDialogueIndex]);
			CurrentDialogueIndex++;
		}
	}
}