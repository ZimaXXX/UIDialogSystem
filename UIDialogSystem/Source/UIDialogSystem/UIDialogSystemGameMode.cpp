// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIDialogSystemGameMode.h"
#include "UIDialogSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUIDialogSystemGameMode::AUIDialogSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
