#include <DxLib.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Assert.h"
#include "Geometry.h"
#include "Item.h"
#include "GameScene.h"
#include "HitClass.h"
#include "ItemFactory.h"

using namespace std;
using Item_ptr = std::shared_ptr<Item>;
typedef std::list<std::shared_ptr<Item>> ItemList_t;

ItemFactory::ItemFactory()
{
}


ItemFactory::~ItemFactory()
{
}

std::weak_ptr<Item>ItemFactory::Create(ITEM_TYPE item,Position2 pos)
{
	switch (item) {
	case ITEM_FEVER:
		_itemList.push_back(Item_ptr());
		break;
	default:
		break;
	}
	return _itemList.back();
}

void ItemFactory::Updata()
{
	for (auto& it : _itemList) {
		it->Updata();
	}
}
void ItemFactory::Draw(Position2 offset) 
{
	for (auto& it : _itemList) {
		it->Draw(offset);
	}
}
ItemList_t& ItemFactory::ItemList()
{
	return _itemList;
}