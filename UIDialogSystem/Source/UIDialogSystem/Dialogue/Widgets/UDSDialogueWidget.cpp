#include "UDSDialogueWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "UIDialogSystem/Dialogue/Data/UDSCharacterConfigDataAsset.h"
#include "UIDialogSystem/Dialogue/Data/UDSHoverKeywordRow.h"

void UUDSDialogueWidget::SetDialogue(const FUDSDialogueEntry& DialogueEntry)
{
    if(DialogueEntry.CharacterConfig.IsNull())
    {
        return;
    }
    if (!DialogueEntry.CharacterConfig.Get())
    {
        DialogueEntry.CharacterConfig.LoadSynchronous();
    }
    UUDSCharacterConfigDataAsset* CharacterConfig = DialogueEntry.CharacterConfig.Get();
    if (CharacterImage)
    {
        if (!CharacterConfig->CharacterPortrait.IsNull() && !CharacterConfig->CharacterPortrait.Get())
        {
            CharacterConfig->CharacterPortrait.LoadSynchronous();
        }
        CharacterImage->SetBrushFromTexture(CharacterConfig->CharacterPortrait.Get());
    }
    if (CharacterNameText)
    {
        CharacterNameText->SetText(CharacterConfig->CharacterName);
    }

    if (DialogueRichText)
    {
        DialogueRichText->SetText(FText::GetEmpty());
    }

    FullDialogueText = DialogueEntry.DialogueText;
    CurrentDialogueString = "";
    DialogueIndex = 0;
    StartTypewriterEffect(TypingRate);
}

TArray<FUDSHoverKeywordRow*> UUDSDialogueWidget::GetHoverKeywords()
{
    if(CachedKeywords.Num() > 0)
    {
        return CachedKeywords;
    }
    TArray<FUDSHoverKeywordRow*> KeywordRows;
    FString ContextString;
    KeywordsDataTable->GetAllRows<FUDSHoverKeywordRow>(TEXT(""), KeywordRows);
    CachedKeywords = KeywordRows;
    return KeywordRows;
}

FString UUDSDialogueWidget::AppendRichTextFormatting(FString CurrentFormattedString)
{
    TArray<FUDSHoverKeywordRow*> KeywordRows = GetHoverKeywords();
    TArray<FString> HighlightedWords;
    for(FUDSHoverKeywordRow* KeywordRow : KeywordRows)
    {
        HighlightedWords.Add(KeywordRow->Keyword.ToString());
    }
    FString AdaptedFormattedDialogueString = {};
    if(CurrentTagStartIndex != INDEX_NONE)
    {
        AdaptedFormattedDialogueString = CurrentFormattedString.Mid(0,  CurrentTagStartIndex);
    }
    else
    {
        //return &CurrentDialogueString[DialogueIndex];
        AdaptedFormattedDialogueString = CurrentFormattedString;
    }    
    TCHAR CurrentLetter = CurrentDialogueString[DialogueIndex];
    if(FChar::IsWhitespace(CurrentLetter) || FChar::IsPunct(CurrentLetter))
    {
        CurrentTagStartIndex = INDEX_NONE;
        return CurrentFormattedString + CurrentLetter;
    }
    
    FString Prefix;
    FString Postfix;
    FString LastPrefix;
    FString LastPostix;
    FString CleanWord = GetWordFromString(FullDialogueText.ToString(), Prefix, Postfix, DialogueIndex);
    FString LastCleanWord = GetWordFromString(CurrentDialogueString, LastPrefix, LastPostix, DialogueIndex);
    int32 KeywordRowIndex = INDEX_NONE;
    bool bIsHighlighted = false;
    if(LastCleanWord.Len() > 0)
    {
        KeywordRowIndex = HighlightedWords.IndexOfByKey(CleanWord);
        bIsHighlighted = KeywordRowIndex != INDEX_NONE;
    }
    FString StyledText = AdaptedFormattedDialogueString; // Build the formatted text dynamically
    if (bIsHighlighted) 
    {
        FString Style = KeywordRows[KeywordRowIndex]->StyleFromDataTable;
        //StyledText += FString::Printf(TEXT("<tooltip text=\"Some Infos\">%s</>"), *CleanWord);
        if(CleanWord != LastCleanWord)
        {
            CurrentTagStartIndex = AdaptedFormattedDialogueString.Len();
        }
        else
        {
            CurrentTagStartIndex = INDEX_NONE;
        }
       
        StyledText += FString::Printf(TEXT("%s<Tooltip TTDesc=\"%s\">%s</>%s"), *LastPrefix, *KeywordRows[KeywordRowIndex]->Description.ToString(), *LastCleanWord, *LastPostix);
    }
    else
    {
        StyledText = StyledText.AppendChar(CurrentDialogueString[DialogueIndex]);
        CurrentTagStartIndex = INDEX_NONE;
    }
    
    return StyledText;
}

FString UUDSDialogueWidget::ApplyRichTextFormatting()
{
    TArray<FUDSHoverKeywordRow*> KeywordRows = GetHoverKeywords();
    TArray<FString> HighlightedWords;
    for(FUDSHoverKeywordRow* KeywordRow : KeywordRows)
    {
        HighlightedWords.Add(KeywordRow->Keyword.ToString());
    }
    FString StyledText = ""; // Build the formatted text dynamically
    FString CurrentWord = ""; // Store the currently typed word
    
    // Split into words
    TArray<FString> Words;
    CurrentDialogueString.ParseIntoArray(Words, TEXT(" "), true); // Split into words
    int32 NumWords = Words.Num();

    for (int32 i = 0; i < NumWords; i++)
    {
        FString Word = Words[i];
        FString Prefix;
        FString Postfix;
        FString CleanWord = GetWordFromString(Word, Prefix, Postfix);
        int32 KeywordRowIndex = INDEX_NONE;
        bool IsHighlighted = false;
        KeywordRowIndex = HighlightedWords.IndexOfByKey(CleanWord);
        IsHighlighted = KeywordRowIndex != INDEX_NONE;
        // If we are currently typing a word, check if it starts as a highlighted word
        if(i == NumWords - 1) // Last word is still being typed
        {
            FString LastPrefix;
            FString LastPostix;
            FString LastCleanWord = GetWordFromString(FullDialogueText.ToString(), LastPrefix, LastPostix, DialogueIndex);
            if(LastCleanWord.Len() > 0 && LastCleanWord != CleanWord)
            {
                KeywordRowIndex = HighlightedWords.IndexOfByKey(LastCleanWord);
                IsHighlighted = KeywordRowIndex != INDEX_NONE;
            }
        }
        // Apply the appropriate formatting
        if (IsHighlighted) 
        {
            FString Style = KeywordRows[KeywordRowIndex]->StyleFromDataTable;
            //StyledText += FString::Printf(TEXT("<tooltip text=\"Some Infos\">%s</>"), *CleanWord);
            StyledText += FString::Printf(TEXT("%s<Tooltip TTDesc=\"%s\">%s</>%s "), *Prefix, *KeywordRows[KeywordRowIndex]->Description.ToString(), *CleanWord, *Postfix);
        }
        else
        {
            StyledText += Word + " ";
        }
    }

    return StyledText.TrimEnd(); // Remove trailing space
}

FString UUDSDialogueWidget::GetWordFromString(const FString& Text, FString& OutPrefix, FString& OutPostfix, int32 Index)
{
    if (Text.IsEmpty() || Index < 0 || Index >= Text.Len())
    {
        return TEXT(""); // Return empty if index is out of bounds
    }
    
    int32 WordStartIndex = 0;
    int32 WordEndIndex = 0;
    int32 PrefixIndex = Index;
    int32 PostfixIndex = 0;

    bool bLetterFound = false;//The word can contain whitespace at the end
    //Find initial character of the word
    
    while (PrefixIndex >= 0)
    {
        TCHAR CurrentLetter = Text[PrefixIndex];
        if(bLetterFound && (FChar::IsWhitespace(CurrentLetter) || FChar::IsPunct(CurrentLetter)))//If we found non-letter character don't include it in word
        //if(bLetterFound && FChar::IsWhitespace(CurrentLetter))//If we found non-letter character don't include it in word
        {
            PrefixIndex++;
            break;
        }
        if(!bLetterFound && FChar::IsAlnum(CurrentLetter))
        {
            bLetterFound = true;            
        }
        PrefixIndex--;        
    }
    
    WordStartIndex = FMath::Max(PrefixIndex, 0);

    //Find initial letter of the word
    while(WordStartIndex < Text.Len() && !FChar::IsAlnum(Text[WordStartIndex]))
    {
        WordStartIndex++;
    }

    WordEndIndex = WordStartIndex;

    //Find last letter of the word
    while(WordEndIndex < Text.Len() && FChar::IsAlnum(Text[WordEndIndex]))
    {
        WordEndIndex++;
    }

    PostfixIndex = WordEndIndex;

    //Find last character of the word
    while(PostfixIndex < Text.Len() && !FChar::IsWhitespace(Text[PostfixIndex]))
    {
        PostfixIndex++;
    }
    
    // Extract the clean word from the text
    OutPrefix = Text.Mid(PrefixIndex, WordStartIndex - PrefixIndex);
    OutPostfix = Text.Mid(WordEndIndex, PostfixIndex - WordEndIndex);
    FString ReturnText = Text.Mid(WordStartIndex, WordEndIndex - WordStartIndex);
    //UE_LOG(LogTemp, Log, TEXT("Prefix: %s, Word: %s, Postfix: %s"), *OutPrefix, *ReturnText, *OutPostfix);
    return ReturnText;
}

void UUDSDialogueWidget::StartTypewriterEffect(float Rate)
{
    bIsTypingFinished = false;
    GetWorld()->GetTimerManager().SetTimer(TypewriterTimerHandle, this, &UUDSDialogueWidget::UpdateTypewriterEffect, Rate, true);
}

void UUDSDialogueWidget::SkipTypewriterEffect()
{
    GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
    if (DialogueRichText) DialogueRichText->SetText(FullDialogueText);
    DialogueIndex = FullDialogueText.ToString().Len();
    UpdateTypewriterEffect();
}

void UUDSDialogueWidget::UpdateTypewriterEffect()
{
    if (DialogueIndex < FullDialogueText.ToString().Len())
    {
        CurrentDialogueString += FullDialogueText.ToString()[DialogueIndex];
        FString FormattedString = DialogueRichText->GetText().ToString();
        FormattedString = AppendRichTextFormatting(FormattedString);
        //FString FormattedString = ApplyRichTextFormatting();
        DialogueRichText->SetText(FText::FromString(FormattedString));
        DialogueIndex++;
        OnRichTextUpdatedDelegate.Broadcast();
    }
    else
    {
        CurrentDialogueString = FullDialogueText.ToString();
        FString FormattedString = ApplyRichTextFormatting();
        DialogueRichText->SetText(FText::FromString(FormattedString));
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
        OnRichTextUpdatedDelegate.Broadcast();
        OnTypingFinished();
    }
}

FString UUDSDialogueWidget::GetCurrentlyTypedWord()
{
    FString ProcessedText = CurrentDialogueString.TrimStartAndEnd();
    int32 LastSpaceIndex;

    if (ProcessedText.FindLastChar(' ', LastSpaceIndex))
    {
        return ProcessedText.Mid(LastSpaceIndex + 1); // Extract last word
    }
    return ProcessedText; // If no space found, return the full typed text
}

void UUDSDialogueWidget::OnKeywordHovered(const FText& Keyword)
{
    if (FullDialogueText.ToString().Contains(Keyword.ToString()))
    {
        UE_LOG(LogTemp, Log, TEXT("Hovered Keyword: %s"), *Keyword.ToString());
    }
}

void UUDSDialogueWidget::OnKeywordUnhovered()
{
}

void UUDSDialogueWidget::OnTypingFinished()
{
    bIsTypingFinished = true;
    OnTypingFinishedDelegate.Broadcast();
}
