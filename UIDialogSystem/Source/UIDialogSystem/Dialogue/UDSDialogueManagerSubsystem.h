#pragma once
		
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/UDSDialogueWidget.h"
#include "UDSDialogueManagerSubsystem.generated.h"

class UUDSDialogueDataAsset;
// Subsystem class for managing dialogue sequences
UCLASS()
class UIDIALOGSYSTEM_API UUDSDialogueManagerSubsystem : public UGameInstanceSubsystem
{
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	GENERATED_BODY()

public:
	// Constructor for the dialogue manager subsystem
	UUDSDialogueManagerSubsystem();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Init(TSubclassOf<class UUDSDialogueWidget> InDialogueWidgetClass, UDataTable* InHoverKeywordsDataTable);

	// Starts the dialogue sequence with the provided dialogue data asset
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset);

	// The hover keywords data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	UDataTable* HoverKeywordsDataTable;
private:
	// Class of the dialogue widget to be used
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<class UUDSDialogueWidget> DialogueWidgetClass;

	// Currently active dialogue widget
	UPROPERTY()
	UUDSDialogueWidget* CurrentDialogueWidget;

	// Queue of dialogue entries to be displayed
	TArray<FUDSDialogueEntry> DialogueQueue;

	// Index of the current dialogue entry being displayed
	int32 CurrentDialogueIndex;

	void ShowNextDialogue();
};