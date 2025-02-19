#pragma once
	
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UDSDialogueWidget.generated.h"

class UUDSCharacterConfigDataAsset;
// Struct for a single dialogue entry
USTRUCT(BlueprintType)
struct FUDSDialogueEntry
{
	GENERATED_BODY()

	// The character configuration data asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UUDSCharacterConfigDataAsset> CharacterConfig;

	// The dialogue text to be displayed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueText;
};
UCLASS()
class UIDIALOGSYSTEM_API UUDSDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets the dialogue entry to be displayed in the widget
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetDialogue(const FUDSDialogueEntry& DialogueEntry);

	// Starts the typewriter effect for displaying dialogue text
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartTypewriterEffect(float Speed);

	// Skips the typewriter effect and displays the full dialogue text immediately
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SkipTypewriterEffect();

private:
	// Image component for displaying the character's portrait
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage;

	// Text block for displaying the character's name
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;

	// Rich text block for displaying the dialogue text with formatting
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* DialogueRichText;

	// Full dialogue text to be displayed
	FText FullDialogueText;
	
	// Current dialogue string being displayed by the typewriter effect
	FString CurrentDialogueString;
	
	// Index of the current character in the dialogue text being displayed
	int32 DialogueIndex = 0;
	
	// Timer handle for the typewriter effect
	FTimerHandle TypewriterTimerHandle;

	// Updates the typewriter effect by adding the next character to the dialogue string
	void UpdateTypewriterEffect();
	
	// Handles keyword hover events to display tooltips
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void OnKeywordHovered(const FText& Keyword);

	// Handles keyword unhover events to hide tooltips
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void OnKeywordUnhovered();
};