#include "ItemData.h"

FItemData::FItemData(): Category(), Icon(nullptr), bCanBeUsed(false), bCanBeStacked(false)
{
	Name.FromString("Undefined");
	Description.FromString("Undefined");
	UseText.FromString("Undefined");
}
