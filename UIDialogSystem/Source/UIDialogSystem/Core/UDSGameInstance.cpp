#include "UDSGameInstance.h"

#include "UIDialogSystem/Dialogue/UDSDialogueManagerSubsystem.h"

void UUDSGameInstance::InitializeDialogueSubsystem(UUDSDialogueDataAsset* DialogueDataAsset, UDataTable* KeywordsDataTable)
{
	if (UUDSDialogueManagerSubsystem* DialogueSubsystem = GetSubsystem<UUDSDialogueManagerSubsystem>())
	{
		DialogueSubsystem->StartDialogue(DialogueDataAsset, KeywordsDataTable);
	}
}	