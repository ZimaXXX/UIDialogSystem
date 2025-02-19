#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UDSDialogueDataAsset.generated.h"

struct FUDSDialogueEntry;

UCLASS()
class UIDIALOGSYSTEM_API UUDSDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Array of dialogue entries
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FUDSDialogueEntry> DialogueEntries;
};