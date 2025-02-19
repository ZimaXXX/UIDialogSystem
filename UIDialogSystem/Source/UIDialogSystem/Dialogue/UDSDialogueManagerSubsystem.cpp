#include "UDSDialogueManagerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Data/UDSDialogueDataAsset.h"
#include "Widgets/UDSDialogueWidget.h"

void UUDSDialogueManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

UUDSDialogueManagerSubsystem::UUDSDialogueManagerSubsystem()
{
}

void UUDSDialogueManagerSubsystem::Init(TSubclassOf<class UUDSDialogueWidget> InDialogueWidgetClass,
	UDataTable* InHoverKeywordsDataTable)
{
	DialogueWidgetClass = InDialogueWidgetClass;
	HoverKeywordsDataTable = InHoverKeywordsDataTable;
}

void UUDSDialogueManagerSubsystem::StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset)
{
	if (DialogueDataAsset)
	{
		DialogueQueue = DialogueDataAsset->DialogueEntries;
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