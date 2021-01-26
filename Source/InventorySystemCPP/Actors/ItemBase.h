#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InventorySystemCPP/Structs/ItemData.h"
#include "ItemBase.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AItemBase : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemData")
	FItemData ItemData;
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse();
};
