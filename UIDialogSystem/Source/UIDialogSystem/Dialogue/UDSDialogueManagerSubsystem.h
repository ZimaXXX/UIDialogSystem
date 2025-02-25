#pragma once
		
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/UDSDialogueWidget.h"
#include "UDSDialogueManagerSubsystem.generated.h"

class UUDSDialogueDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueWidgetCreated, UUDSDialogueWidget*, DialogueWidget);

// Subsystem class for managing dialogue sequences
UCLASS()
class UIDIALOGSYSTEM_API UUDSDialogueManagerSubsystem : public UWorldSubsystem
{
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	GENERATED_BODY()

public:
	// Constructor for the dialogue manager subsystem
	UUDSDialogueManagerSubsystem();
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueWidgetCreated OnDialogueWidgetCreatedDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Init(TSubclassOf<class UUDSDialogueWidget> InDialogueWidgetClass, UDataTable* InHoverKeywordsDataTable);

	// Starts the dialogue sequence with the provided dialogue data asset
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset);
	
	// Function to display the next dialogue entry in the queue
	UFUNCTION()
	void ShowNextDialogue();

	// Function to skip the current dialogue entry or typewriter effect
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SkipCurrentDialogue();

protected:
	// The hover keywords data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	TObjectPtr<UDataTable> HoverKeywordsDataTable;

	// Currently active dialogue widget
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UUDSDialogueWidget> CurrentDialogueWidget;
	
	// Class of the dialogue widget to be used
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<class UUDSDialogueWidget> DialogueWidgetClass;

	// Queue of dialogue entries to be displayed
	TArray<FUDSDialogueEntry> DialogueQueue;

	// Index of the current dialogue entry being displayed
	int32 CurrentDialogueIndex = -1;
	
	// Function to handle changes in culture/language settings
	void HandleCultureChanged();
};