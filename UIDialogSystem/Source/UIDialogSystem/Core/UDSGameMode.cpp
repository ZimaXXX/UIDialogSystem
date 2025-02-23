// Fill out your copyright notice in the Description page of Project Settings.


#include "UDSGameMode.h"

#include "UIDialogSystem/Dialogue/UDSDialogueManagerSubsystem.h"

void AUDSGameMode::StartDialogueSubsystem(UUDSDialogueDataAsset* DialogueDataAsset)
{
	if (UUDSDialogueManagerSubsystem* DialogueSubsystem = GetWorld()->GetSubsystem<UUDSDialogueManagerSubsystem>())
	{
		DialogueSubsystem->StartDialogue(DialogueDataAsset);
	}
}