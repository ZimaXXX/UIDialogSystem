#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UDSHoverKeywordRow.generated.h"

USTRUCT(BlueprintType)
struct FUDSHoverKeywordRow : public FTableRowBase
{
	GENERATED_BODY()

	// The keyword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	FText Keyword;
	
	// The description for the keyword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	FText Description;
};