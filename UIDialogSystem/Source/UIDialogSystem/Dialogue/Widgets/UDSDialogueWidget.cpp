#include "UDSDialogueWidget.h"
	#include "Components/Image.h"
	#include "Components/TextBlock.h"
	#include "Components/RichTextBlock.h"
#include "UIDialogSystem/Dialogue/Data/UDSCharacterConfigDataAsset.h"

void UUDSDialogueWidget::SetDialogue(const FUDSDialogueEntry& DialogueEntry)
{
	if(!DialogueEntry.CharacterConfig.IsValid())
	{
		DialogueEntry.CharacterConfig.LoadSynchronous();
	}
	UUDSCharacterConfigDataAsset* CharacterConfig = DialogueEntry.CharacterConfig.Get();
	if (CharacterImage)
	{		
		if(!CharacterConfig->CharacterPortrait.IsValid())
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
	StartTypewriterEffect(0.05f);
}

void UUDSDialogueWidget::StartTypewriterEffect(float Speed)
{
	GetWorld()->GetTimerManager().SetTimer(TypewriterTimerHandle, this, &UUDSDialogueWidget::UpdateTypewriterEffect, Speed, true);
}

void UUDSDialogueWidget::SkipTypewriterEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
	if (DialogueRichText) DialogueRichText->SetText(FullDialogueText);
}

void UUDSDialogueWidget::UpdateTypewriterEffect()
{
	if (DialogueIndex < FullDialogueText.ToString().Len())
	{
		CurrentDialogueString += FullDialogueText.ToString()[DialogueIndex];
		DialogueRichText->SetText(FText::FromString(CurrentDialogueString));
		DialogueIndex++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
	}
}

void UUDSDialogueWidget::OnKeywordHovered(const FText& Keyword)
{
	// Display tooltip for the hovered keyword
	if (FullDialogueText.ToString().Contains(Keyword.ToString()))
	{
		// Show tooltip (implementation depends on your UI framework)
		// Example: Create and display a tooltip widget
		UE_LOG(LogTemp, Log, TEXT("Hovered Keyword: %s"), *Keyword.ToString());
	}
}

void UUDSDialogueWidget::OnKeywordUnhovered()
{
	// Hide tooltip (implementation depends on your UI framework)
	// Example: Remove or hide the tooltip widget
}