﻿#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "InventorySystemCPP/Structs/ItemData.h"
#include "GameFramework/Actor.h"
#include "InventorySystemCPP/Structs/InventorySlot.h"

#include "Inventory.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AInventory : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<FInventorySlot*> Slots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	int32 SlotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slots")
	int32 MaxStackSize;

	int32 GetAmountAtIndex(const int32 SlotIndex) const;
	bool IsSlotEmpty(int32 SlotIndex) const;
	bool EmptySlotExists(int32& SlotIndex) const;
	bool NotFullStackExists(TSubclassOf<AItemBase> ItemClass, int32& SlotIndex) const;
	void UpdateSlot(int32 SlotIndex, TSubclassOf<AItemBase> ItemClass, int32 Amount);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUpdateSlotAtIndex"))
	void OnUpdateSlotAtIndex(int32 SlotIndex);
	
public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(TSubclassOf<AItemBase> ItemClass, int32 AmountToAdd, int32& RemainingAmount);
	UFUNCTION(BlueprintCallable)
    bool GetSlotAtIndex(int32 SlotIndex, FItemData& ItemData, int32& Amount) const;
	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 SlotIndex1, int32 SlotIndex2);
	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromIndex(int32 SlotIndex, int32 AmountToRemove);
	UFUNCTION(BlueprintCallable)
	bool SplitStack(int32 SlotIndex, int32 AmountToSplit);
	UFUNCTION(BlueprintCallable)
	bool UseItemFromIndex(int32 SlotIndex);
	UFUNCTION(BlueprintCallable)
	bool CombineStacks(int32 FromIndex, int32 ToIndex);
	UFUNCTION(BlueprintCallable)
	bool HandleDragDropOperation(int32 FromIndex, int32 ToIndex);
};
