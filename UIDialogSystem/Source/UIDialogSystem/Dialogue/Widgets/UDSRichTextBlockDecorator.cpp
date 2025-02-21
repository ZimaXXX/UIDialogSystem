// Fill out your copyright notice in the Description page of Project Settings.


#include "UDSRichTextBlockDecorator.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"


// Class charged of creating the inline tooltip
class FUDDSRichInlineTooltip : public FRichTextDecorator
{
public:
	FUDDSRichInlineTooltip(URichTextBlock* InOwner, const FTextBlockStyle& InTextStyle, const FTextBlockStyle& InTooltipTextStyle)
		: FRichTextDecorator(InOwner)
		, TextStyle(InTextStyle)
		, TooltipTextStyle(InTooltipTextStyle)
	{
	}

	// Only valid if text is: <tooltip text="Some infos">Some text</>
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return RunParseResult.Name == TEXT("tooltip") && RunParseResult.MetaData.Contains(TEXT("text"));
	}

protected:
	/**
	 * Create a STextBlock with a tooltip text.
	 * 
	 * For <Tooltip Text="Some infos">Some text</>:
	 * - RunInfo.Content is "Some text"
	 * - RunInfo.MetaData[TEXT("text")] is "Some infos"
	 */
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& InRunInfo, const FTextBlockStyle& InTextStyle) const override
	{
		// Check if we already created a widget for this RunInfo
		if(!CachedWidget.Get())
		{
			CachedWidget = SNew(SToolTip);	
		}
		if(InRunInfo.MetaData[TEXT("text")] != LastTooltipText)
		{
			CachedWidget = SNew(SToolTip);	
			CachedWidget->SetContentWidget(
				SNew(STextBlock)
				.Text(FText::FromString(InRunInfo.MetaData[TEXT("text")]))
				.TextStyle(&TooltipTextStyle));
		}

		LastTooltipText = InRunInfo.MetaData[TEXT("text")];
		// TSharedRef<STextBlock> TooltipTextBlock = StaticCastSharedRef<STextBlock>(CachedWidget->GetContentWidget());// static_cast< STextBlock* >(Tooltip->GetContentWidget().Get());
		// TooltipTextBlock->SetText(FText::FromString(InRunInfo.MetaData[TEXT("text")]));
		// TooltipTextBlock->SetTextStyle(&TooltipTextStyle);

		
		return SNew(STextBlock)
		.Text(InRunInfo.Content)
		.TextStyle(&TextStyle)
		.ToolTip(CachedWidget);
		
		// if (!CachedWidget.Get())
		// {		
		// 	CachedWidget = SNew(STextBlock)
		// 	.Text(InRunInfo.Content)
		// 	.TextStyle(&TextStyle)
		// 	.ToolTip(SNew(SToolTip)
		// 	[
		// 		SNew(STextBlock)
		// 		.Text(FText::FromString(InRunInfo.MetaData[TEXT("text")]))
		// 		.TextStyle(&TooltipTextStyle)
		// 	]);		
		// }
		// else
		// {
		// 	STextBlock* TextBlock = CachedWidget.Get();
		// 	TextBlock->SetText(InRunInfo.Content);
  //           TextBlock->SetTextStyle(&TextStyle);
		// 	TSharedPtr<IToolTip> Tooltip = TextBlock->GetToolTip();
		// 	TSharedRef<STextBlock> TooltipTextBlock = StaticCastSharedRef<STextBlock>(Tooltip->GetContentWidget());// static_cast< STextBlock* >(Tooltip->GetContentWidget().Get());
		// 	TooltipTextBlock->SetText(FText::FromString(InRunInfo.MetaData[TEXT("text")]));
		// 	TooltipTextBlock->SetTextStyle(&TooltipTextStyle);
		// }
		// return CachedWidget;
	}

private:
	FTextBlockStyle TextStyle;
	FTextBlockStyle TooltipTextStyle;
	mutable TSharedPtr<IToolTip> CachedWidget;
	mutable FString LastTooltipText;
};

// UUDSRichTextBlockDecorator

UUDSRichTextBlockDecorator::UUDSRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

// Return our custom class for creating the inline widget
TSharedPtr<ITextDecorator> UUDSRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FUDDSRichInlineTooltip(InOwner, TextStyle, TooltipTextStyle));
}
