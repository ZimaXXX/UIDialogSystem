#include "UDSGameInstance.h"

#include "UIDialogSystem/Dialogue/UDSDialogueManagerSubsystem.h"

void UUDSGameInstance::StartDialogueSubsystem(UUDSDialogueDataAsset* DialogueDataAsset)
{
	if (UUDSDialogueManagerSubsystem* DialogueSubsystem = GetSubsystem<UUDSDialogueManagerSubsystem>())
	{
		DialogueSubsystem->StartDialogue(DialogueDataAsset);
	}
}