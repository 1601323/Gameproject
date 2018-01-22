#pragma once

#include <list>
#include <memory>
#include <map>
#include "Item.h"

class GameScene;
class Player;
class Item;
class HitClass;

class ItemFactory :
	public Item
{
	friend GameScene;
private:
	ItemFactory();
	HitClass* _hit;
	typedef std::list<std::shared_ptr<Item>> ItemList_t;
	ItemList_t _itemList;
	std::weak_ptr<Item>ItemFactory::Create(ITEM_TYPE item,Position2 pos);

public:
	~ItemFactory();
	void Updata();
	void Draw(Position2 offset);
	ItemList_t& ItemList();
};

