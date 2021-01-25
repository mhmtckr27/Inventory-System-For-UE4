#pragma once

#include "CoreMinimal.h"
#include "InventorySystemCPP/Enums/ItemCategory.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYSYSTEMCPP_API FItemData
{
	GENERATED_USTRUCT_BODY()
public:
	FItemData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UseText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemCategory> Category;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeUsed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeStacked;
};
