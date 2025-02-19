#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UDSCharacterConfigDataAsset.generated.h"

UCLASS()
class UIDIALOGSYSTEM_API UUDSCharacterConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// The name of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FText CharacterName;

	// The portrait image of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterPortrait;
};