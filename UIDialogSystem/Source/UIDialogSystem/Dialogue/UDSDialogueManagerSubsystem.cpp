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

void UUDSDialogueManagerSubsystem::HandleCultureChanged()
{
    if (CurrentDialogueWidget)//rewrite current dialogue in updated language
    {
        CurrentDialogueWidget->SetDialogue(DialogueQueue[CurrentDialogueIndex]);
    }
}

void UUDSDialogueManagerSubsystem::Init(TSubclassOf<class UUDSDialogueWidget> InDialogueWidgetClass,
                                        UDataTable* InHoverKeywordsDataTable)
{
    DialogueWidgetClass = InDialogueWidgetClass;
    HoverKeywordsDataTable = InHoverKeywordsDataTable;
    FInternationalization::Get().OnCultureChanged().AddUObject(this, &UUDSDialogueManagerSubsystem::HandleCultureChanged);
}

void UUDSDialogueManagerSubsystem::StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset)
{
    if (DialogueDataAsset)
    {
        DialogueQueue = DialogueDataAsset->DialogueEntries;
        CurrentDialogueIndex = -1;
        ShowNextDialogue();
    }
}

void UUDSDialogueManagerSubsystem::ShowNextDialogue()
{
    if (CurrentDialogueIndex < DialogueQueue.Num() - 1)
    {
        if (!CurrentDialogueWidget)//caching widget for performance
        {
            if (DialogueWidgetClass)
            {
                CurrentDialogueWidget = CreateWidget<UUDSDialogueWidget>(GetWorld(), DialogueWidgetClass);
                CurrentDialogueWidget->Init(HoverKeywordsDataTable);
                OnDialogueWidgetCreatedDelegate.Broadcast(CurrentDialogueWidget);
            }
        }

        if (CurrentDialogueWidget)
        {
            CurrentDialogueIndex++;
            CurrentDialogueWidget->SetDialogue(DialogueQueue[CurrentDialogueIndex]);
        }
    }
}

void UUDSDialogueManagerSubsystem::SkipCurrentDialogue()
{
    if(!CurrentDialogueWidget)
    {
        return;
    }
    if(!CurrentDialogueWidget->IsTypingFinished())//if still typing, finish it
    {
        CurrentDialogueWidget->SkipTypewriterEffect();
    }
    else if(!CurrentDialogueWidget->IsScrolledToEnd())//if not scrolled to end, scroll to end
    {
        CurrentDialogueWidget->ScrollToEnd();
    }
    else//if scrolled to end, show next dialogue
    {
        ShowNextDialogue();
    }
}
