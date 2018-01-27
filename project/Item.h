#pragma once
#include "Object.h"
class HitClass;
class Player;
class Rope;

class Item :
	public Object
{
private:
	HitClass* _hit;
public:
	Item();
	~Item();
	Rect _itemRect;
	virtual void Draw(Position2 offset) =0;

	Rect& GetRect();
};

