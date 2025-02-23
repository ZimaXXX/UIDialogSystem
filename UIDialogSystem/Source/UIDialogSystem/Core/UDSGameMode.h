// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UDSGameMode.generated.h"

class UUDSDialogueDataAsset;
/**
 * 
 */
UCLASS()
class UIDIALOGSYSTEM_API AUDSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Function to initialize the dialogue subsystem with a data asset
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueSubsystem(UUDSDialogueDataAsset* DialogueDataAsset);
};
