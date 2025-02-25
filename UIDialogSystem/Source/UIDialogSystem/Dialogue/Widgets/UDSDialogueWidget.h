#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UDSDialogueWidget.generated.h"

struct FUDSHoverKeywordRow;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTypingFinishedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRichTextUpdated);

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
    //delegates
    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnTypingFinishedDelegate OnTypingFinishedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnRichTextUpdated OnRichTextUpdatedDelegate;

    //methods
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SetDialogue(const FUDSDialogueEntry& DialogueEntry);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartTypewriterEffect(float Rate);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SkipTypewriterEffect();

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool IsScrolledToEnd();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void ScrollToEnd();

    UFUNCTION(BlueprintPure)
    bool IsTypingFinished() const { return bIsTypingFinished;}

    void Init(const TObjectPtr<UDataTable>& InKeywordsDataTable);

    //variables    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config")
    float TypingRate = 0.05f;

    //widget bindings
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* CharacterImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* CharacterNameText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class URichTextBlock* DialogueRichText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UScrollBox* ScrollBox;
    
protected:
    //methods
    void UpdateTypewriterEffect();
    UFUNCTION()
    void OnTypingFinished();
    
    //obtain keywords from DataTable. Cached for optimization
    TArray<FUDSHoverKeywordRow*> GetHoverKeywords();
    
    //method optimized for adding single letter and retain formatting
    FString AppendRichTextFormatting(FString CurrentFormattedString);

    //method optimized to format whole dialogue text
    FString ApplyRichTextFormatting();

    //word can be anything that doesn't contain whitepsace, so prefix and postifx are meant to contain special signs
    FString GetWordFromString(const FString& Text, FString& OutPrefix, FString& OutPostfix, int32 Index = 0);
    
    //variables
    FText FullDialogueText;
    FString CurrentDialogueString;
    int32 DialogueIndex = 0;
    int32 CurrentTagStartIndex = INDEX_NONE;
    FTimerHandle TypewriterTimerHandle;
    bool bIsTypingFinished = true;
    TArray<FUDSHoverKeywordRow*> CachedKeywords;    
    UPROPERTY()
    TObjectPtr<UDataTable> KeywordsDataTable;
    

};