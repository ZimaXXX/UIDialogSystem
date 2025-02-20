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

	// The color for the keyword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	FString StyleFromDataTable;

	// The description for the keyword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keywords")
	FText Description;
};