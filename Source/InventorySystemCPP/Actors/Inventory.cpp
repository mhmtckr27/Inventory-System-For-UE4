#include "Inventory.h"
#include "InventorySystemCPP/Structs/InventorySlot.h"

// Sets default values
AInventory::AInventory()
{
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	Slots.SetNum(SlotCount);
	for (int i = 0; i < SlotCount; i++)
	{
		Slots[i] = new FInventorySlot();
	}
}

int32 AInventory::GetAmountAtIndex(const int32 SlotIndex) const
{
	return Slots[SlotIndex]->Amount;
}

bool AInventory::IsSlotEmpty(const int32 SlotIndex) const
{
	return Slots[SlotIndex]->ItemClass == nullptr;
}

bool AInventory::GetSlotAtIndex(const int32 SlotIndex, FItemData& ItemData, int32& Amount) const
{
	if(Slots[SlotIndex]->ItemClass != nullptr)
	{
		ItemData = Slots[SlotIndex]->ItemClass.GetDefaultObject()->ItemData;
	}
	else
	{
		return false;
	}
	Amount = Slots[SlotIndex]->Amount;	
	return true;
}

bool AInventory::SwapSlots(const int32 SlotIndex1, const int32 SlotIndex2)
{
	if(IsSlotEmpty(SlotIndex1) && IsSlotEmpty(SlotIndex2))
	{
		return false;
	}
	FInventorySlot TempSlot;
	TempSlot.ItemClass = Slots[SlotIndex1]->ItemClass;
	TempSlot.Amount = Slots[SlotIndex1]->Amount;

	UpdateSlot(SlotIndex1, Slots[SlotIndex2]->ItemClass, Slots[SlotIndex2]->Amount);
	UpdateSlot(SlotIndex2, TempSlot.ItemClass, TempSlot.Amount);
	return true;
}

bool AInventory::RemoveItemFromIndex(const int32 SlotIndex, const int32 AmountToRemove)
{
	if(IsSlotEmpty(SlotIndex) || AmountToRemove < 1)
	{
		return false;
	}
	if(AmountToRemove >= Slots[SlotIndex]->Amount)
	{
		UpdateSlot(SlotIndex, nullptr, 0);
	}
	else
	{
		UpdateSlot(SlotIndex, Slots[SlotIndex]->ItemClass, Slots[SlotIndex]->Amount - AmountToRemove);
	}
	return true;
}

bool AInventory::SplitStack(const int32 SlotIndex, const int32 AmountToSplit)
{
	int32 EmptySlotIndex;
	if(IsSlotEmpty(SlotIndex) || !Slots[SlotIndex]->ItemClass.GetDefaultObject()->ItemData.bCanBeStacked || !EmptySlotExists(EmptySlotIndex) || AmountToSplit >= Slots[SlotIndex]->Amount || !RemoveItemFromIndex(SlotIndex, AmountToSplit))
	{
		return false;
	}
	const TSubclassOf<AItemBase> ItemClass = Slots[SlotIndex]->ItemClass;
	UpdateSlot(EmptySlotIndex, ItemClass, AmountToSplit);
	return true;
}

bool AInventory::UseItemFromIndex(const int32 SlotIndex)
{
	if(IsSlotEmpty(SlotIndex) || !Slots[SlotIndex]->ItemClass.GetDefaultObject()->ItemData.bCanBeUsed)
	{
		return false;
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AItemBase* ItemToUse = GetWorld()->SpawnActor<AItemBase>(Slots[SlotIndex]->ItemClass, SpawnParameters);
	if(RemoveItemFromIndex(SlotIndex, 1))
	{
		ItemToUse->OnUse();
		return true;
	}
	return false;
}

bool AInventory::CombineStacks(const int32 FromIndex, const int32 ToIndex)
{
	if(Slots[ToIndex]->Amount + Slots[FromIndex]->Amount > MaxStackSize)
	{
		const int32 RemainingAmount = Slots[ToIndex]->Amount + Slots[FromIndex]->Amount - MaxStackSize;
		UpdateSlot(ToIndex, Slots[ToIndex]->ItemClass, MaxStackSize);
		UpdateSlot(FromIndex, Slots[FromIndex]->ItemClass, RemainingAmount);
	}
	else
	{
		UpdateSlot(ToIndex, Slots[ToIndex]->ItemClass, Slots[ToIndex]->Amount + Slots[FromIndex]->Amount);
		UpdateSlot(FromIndex, nullptr, 0);
	}
	return true;
}

//trys to split 1 item from stack to provided slot.
bool AInventory::SplitStackToIndex(const int32 FromIndex, const int32 ToIndex)
{
	if(Slots[FromIndex]->Amount > 1)
	{
		if(IsSlotEmpty(ToIndex))
		{
			UpdateSlot(FromIndex, Slots[FromIndex]->ItemClass, Slots[FromIndex]->Amount - 1);
			UpdateSlot(ToIndex, Slots[FromIndex]->ItemClass, 1);
		}
		else if(Slots[FromIndex]->ItemClass == Slots[ToIndex]->ItemClass)
		{
			if(Slots[ToIndex]->Amount < MaxStackSize)
			{
				UpdateSlot(FromIndex, Slots[FromIndex]->ItemClass, Slots[FromIndex]->Amount - 1);
				UpdateSlot(ToIndex, Slots[ToIndex]->ItemClass, Slots[ToIndex]->Amount + 1);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool AInventory::HandleDragDropOperation(const int32 FromIndex, const int32 ToIndex, const bool bIsSplitButtonDown)
{
	//if splitting stack is intended with holding the split button down (default is left shift)
	if(bIsSplitButtonDown && Slots[FromIndex]->ItemClass.GetDefaultObject()->ItemData.bCanBeStacked)
	{
		return SplitStackToIndex(FromIndex, ToIndex);
	}
	//if item types are not same OR second slot is empty, move first slot to second slot.
	if(Slots[FromIndex]->ItemClass != Slots[ToIndex]->ItemClass)
	{
		SwapSlots(FromIndex, ToIndex);
	}
	//items are not stackable, no need to do anything
	else if(!Slots[ToIndex]->ItemClass.GetDefaultObject()->ItemData.bCanBeStacked)
	{
		return false;
	}
	//if item types are same AND they are stackable, but one of the slots is full, swap slots.
	else if(Slots[FromIndex]->Amount == MaxStackSize || Slots[ToIndex]->Amount == MaxStackSize)
	{
		SwapSlots(FromIndex, ToIndex);
	}
	//if item types are same AND they are stackable, but both slots are full, no need to do anything
	else if(Slots[FromIndex]->Amount == MaxStackSize && Slots[ToIndex]->Amount == MaxStackSize)
	{
		return false;
	}
	else
	{
		CombineStacks(FromIndex, ToIndex);
	}
	return true;
}

bool AInventory::EmptySlotExists(int32& SlotIndex) const
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if(IsSlotEmpty(i))
		{
			SlotIndex = i;
			return true;
		}
	}
	return false;
}

bool AInventory::NotFullStackExists(const TSubclassOf<AItemBase> ItemClass, int32& SlotIndex) const
{
	for(int32 i = 0; i < Slots.Num(); i++)
	{
		if(!IsSlotEmpty(i))
		{
			if(GetAmountAtIndex(i) < MaxStackSize && ItemClass == Slots[i]->ItemClass)
			{
				SlotIndex = i;
				return true;
			}
		}
	}
	return false;
}

void AInventory::UpdateSlot(const int32 SlotIndex, const TSubclassOf<AItemBase> ItemClass, const int32 Amount)
{
	Slots[SlotIndex]->ItemClass = ItemClass;
	Slots[SlotIndex]->Amount = Amount;
	OnUpdateSlotAtIndex(SlotIndex);
}

bool AInventory::AddItem_Implementation(const TSubclassOf<AItemBase> ItemClass, const int32 AmountToAdd, int32& RemainingAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("base"));
	return AddItem_Internal(ItemClass, AmountToAdd, RemainingAmount);
}

bool AInventory::AddItem_Internal(const TSubclassOf<AItemBase> ItemClass, const int32 AmountToAdd, int32& RemainingAmount)
{
	int32 LocalFoundIndex;
	int32 LocalAmountToAdd = AmountToAdd;
	const TSubclassOf<AItemBase> LocalItemClass = ItemClass;
	//if item can be stacked
	if(LocalItemClass.GetDefaultObject()->ItemData.bCanBeStacked)
	{
		//if there is a stack that is not full we will add to that slot
		if(NotFullStackExists(LocalItemClass, LocalFoundIndex))
		{
			int32 LocalTotalAmount = GetAmountAtIndex(LocalFoundIndex) + LocalAmountToAdd;
			//if slot cannot fit the amount we want to add, we will add maxstacksize to that slot,
			//then search another empty slot.
			if(LocalTotalAmount > MaxStackSize)
			{
				LocalAmountToAdd = LocalTotalAmount - MaxStackSize;
				UpdateSlot(LocalFoundIndex, LocalItemClass, MaxStackSize);
				AddItem_Internal(LocalItemClass, LocalAmountToAdd, RemainingAmount);
				return true;
			}
			//if slot can fit the amount we want to add, add to that slot and return.
			else
			{
				UpdateSlot(LocalFoundIndex, LocalItemClass, LocalTotalAmount);
				RemainingAmount = 0;
				return true;
			}
		}
		//if no stack present that is not full, we will look for a free slot.
		else
		{
			//if an empty slot exists, add to that slot.
			if(EmptySlotExists(LocalFoundIndex))
			{
				//if slot cannot fit the amount we want to add, we will add maxstacksize to that slot,
				//then search another empty slot.
				if(LocalAmountToAdd > MaxStackSize)
				{
					UpdateSlot(LocalFoundIndex, LocalItemClass, MaxStackSize);
					AddItem_Internal(LocalItemClass, LocalAmountToAdd - MaxStackSize, RemainingAmount);
					return true;
				}
				//if slot can fit the amount we want to add, add to that slot and return.
				else
				{
					UpdateSlot(LocalFoundIndex, LocalItemClass, LocalAmountToAdd);
					RemainingAmount = 0;
					return true;
				}
			}
			//else inventory is full, we cannot add item(s)
			else
			{
				RemainingAmount = LocalAmountToAdd;
				return false;
			}
		}
	}
	//if item canNOT be stacked
	else
	{
		//if there is an empty slot
		if(EmptySlotExists(LocalFoundIndex))
		{
			UpdateSlot(LocalFoundIndex, LocalItemClass, 1);
			LocalAmountToAdd--;
			//if there are still item(s) to add
			if(LocalAmountToAdd > 0)
			{
				AddItem_Internal(LocalItemClass, LocalAmountToAdd, RemainingAmount);
				//since we added at least 1 item, we return true, instead of return AddItem,
				//because we will show screen notifications.
				return true;
			}
			//if remaining amount is 0, we successfully added the item(s)
			else
			{
				RemainingAmount = 0;
				return true;
			}
		}
		//if there is no empty slot, we canNOT add item 
		else
		{
			RemainingAmount = LocalAmountToAdd;
			return false;
		}
	}
}
