#pragma once

#include "CoreMinimal.h"
#include "InventorySystemCPP/Actors/ItemBase.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYSYSTEMCPP_API FInventorySlot
{
	GENERATED_USTRUCT_BODY()
	
public:
	FInventorySlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};
