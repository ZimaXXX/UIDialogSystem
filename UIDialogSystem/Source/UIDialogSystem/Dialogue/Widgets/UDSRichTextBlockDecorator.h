// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "UDSRichTextBlockDecorator.generated.h"

/**
 * 
 */
UCLASS()
class UIDIALOGSYSTEM_API UUDSRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	UUDSRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	
protected:
	// Style used for text
	UPROPERTY(EditAnywhere, Category=Appearance)
	FTextBlockStyle TextStyle;

	// Style used for tooltip text
	UPROPERTY(EditAnywhere, Category=Appearance)
	FTextBlockStyle TooltipTextStyle;


	TSharedPtr<STextBlock> CachedWidget;
};
