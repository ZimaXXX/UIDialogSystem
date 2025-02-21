#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UDSDialogueWidget.generated.h"

struct FUDSHoverKeywordRow;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTypingFinishedDelegate);

class UUDSCharacterConfigDataAsset;

USTRUCT(BlueprintType)
struct FUDSDialogueEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UUDSCharacterConfigDataAsset> CharacterConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
    FText DialogueText;
};

UCLASS()
class UIDIALOGSYSTEM_API UUDSDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnTypingFinishedDelegate OnTypingFinishedDelegate;
    


    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SetDialogue(const FUDSDialogueEntry& DialogueEntry);
    FString ApplyRichTextFormatting();
    FString GetWordFromString(const FString& Text, FString& OutPrefix, FString& OutPostfix, int32 Index = 0);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartTypewriterEffect(float Rate);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SkipTypewriterEffect();

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Typewriter")
    float TypingRate = 0.05f;

    UFUNCTION(BlueprintPure)
    bool IsTypingFinished() const { return bIsTypingFinished;}

    UPROPERTY()
    UDataTable* KeywordsDataTable;
private:
    UPROPERTY(meta = (BindWidget))
    class UImage* CharacterImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CharacterNameText;

    UPROPERTY(meta = (BindWidget))
    class URichTextBlock* DialogueRichText;

    FText FullDialogueText;
    FString CurrentDialogueString;
    int32 DialogueIndex = 0;
    int32 CurrentTagStartIndex = INDEX_NONE;
    FTimerHandle TypewriterTimerHandle;
    
    void UpdateTypewriterEffect();
    FString GetCurrentlyTypedWord();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnKeywordHovered(const FText& Keyword);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnKeywordUnhovered();

    bool bIsTypingFinished = true;

    UFUNCTION()
    void OnTypingFinished();

    TArray<FUDSHoverKeywordRow*> GetHoverKeywords();
    FString AppendRichTextFormatting(FString CurrentFormattedString);
};