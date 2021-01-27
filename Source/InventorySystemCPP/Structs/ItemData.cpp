#include "ItemData.h"

FItemData::FItemData(): Category(), Icon(nullptr), bCanBeUsed(false), bCanBeStacked(false), Weight(0), Value(0)
{
	Name.FromString("Undefined");
	Description.FromString("Undefined");
	UseText.FromString("Undefined");
}
