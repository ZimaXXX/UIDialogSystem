#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UDSGameInstance.generated.h"

class UUDSDialogueDataAsset;

UCLASS()
class UIDIALOGSYSTEM_API UUDSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Function to initialize the dialogue subsystem with a data asset
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueSubsystem(UUDSDialogueDataAsset* DialogueDataAsset);
};