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

bool AInventory::NotFullStackExists(TSubclassOf<AItemBase> ItemClass, int32& SlotIndex) const
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

bool AInventory::AddItem(const TSubclassOf<AItemBase> ItemClass, const int32 AmountToAdd, int32& RemainingAmount)
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
				AddItem(LocalItemClass, LocalAmountToAdd, RemainingAmount);
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
					AddItem(LocalItemClass, LocalAmountToAdd - MaxStackSize, RemainingAmount);
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
				AddItem(LocalItemClass, LocalAmountToAdd, RemainingAmount);
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
