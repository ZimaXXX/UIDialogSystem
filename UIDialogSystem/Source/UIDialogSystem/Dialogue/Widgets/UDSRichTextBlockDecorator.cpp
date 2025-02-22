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
		return RunParseResult.Name == TEXT("tooltip") && RunParseResult.MetaData.Contains(TEXT("ttdesc"));
	}

protected:
	/**
	 * Create a STextBlock with a tooltip text.
	 * 
	 * For <Tooltip Text="MyTooltipDescription">MyWord</>:
	 * - RunInfo.Content is "MyWord"
	 * - RunInfo.MetaData[TEXT("text")] is "MyTooltipDescription"
	 *
	 * Optimization - Caching Tooltips
	 * RichTextBlock is updated with each typed letter which recreated the tooltip widget each time.
	 * Tooltip Description is checked instead of Word as Word grows until reaching the end ("Scene" is "S", "Sc", "Sce", "Scen", "Scene") but Tooltip Description remains the same.
	 */
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& InRunInfo, const FTextBlockStyle& InTextStyle) const override
	{
		// Check if we already created a widget for this RunInfo
		const FString& CurrentText = InRunInfo.MetaData[TEXT("ttdesc")];
		TSharedPtr<IToolTip> CachedWidget;
		for (TSharedPtr<IToolTip> TempCachedWidget : CachedWidgets)
		{
			if(TempCachedWidget.Get() && StaticCastSharedRef<STextBlock>(TempCachedWidget->GetContentWidget()).Get().GetText().ToString() == CurrentText)
			{
				CachedWidget = TempCachedWidget;
			}
		}

		//Create mew Tooltip and Cache it
		if(!CachedWidget.Get())
		{
			CachedWidget = SNew(SToolTip);
			CachedWidgets.AddUnique(CachedWidget);

			// Update the tooltip text if it has changed
			CachedWidget->SetContentWidget(
				SNew(STextBlock)
				.Text(FText::FromString(CurrentText))
				.TextStyle(&TooltipTextStyle));
		}
		
		return SNew(STextBlock)
		.Text(InRunInfo.Content)
		.TextStyle(&TextStyle)
		.ToolTip(CachedWidget);		
	}

private:
	FTextBlockStyle TextStyle;
	FTextBlockStyle TooltipTextStyle;
	mutable TArray<TSharedPtr<IToolTip>> CachedWidgets;
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
