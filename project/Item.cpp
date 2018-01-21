#include <DxLib.h>
#include <iostream>
#include "HitClass.h"
#include "Player.h"
#include "Item.h"



Item::Item()
{
}


Item::~Item()
{
}

Rect& Item::GetRect()
{
	return _itemRect;
}
