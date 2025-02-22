#pragma once
		
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/UDSDialogueWidget.h"
#include "UDSDialogueManagerSubsystem.generated.h"

class UUDSDialogueDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueWidgetCreated, UUDSDialogueWidget*, DialogueWidget);

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
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueWidgetCreated OnDialogueWidgetCreatedDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Init(TSubclassOf<class UUDSDialogueWidget> InDialogueWidgetClass, UDataTable* InHoverKeywordsDataTable);

	// Starts the dialogue sequence with the provided dialogue data asset
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UUDSDialogueDataAsset* DialogueDataAsset);

	// The hover keywords data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	UDataTable* HoverKeywordsDataTable;

	// Currently active dialogue widget
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	UUDSDialogueWidget* CurrentDialogueWidget;
private:
	// Class of the dialogue widget to be used
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<class UUDSDialogueWidget> DialogueWidgetClass;

	// Queue of dialogue entries to be displayed
	TArray<FUDSDialogueEntry> DialogueQueue;

	// Index of the current dialogue entry being displayed
	int32 CurrentDialogueIndex;

	UFUNCTION()
	void ShowNextDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SkipCurrentDialogue();
};