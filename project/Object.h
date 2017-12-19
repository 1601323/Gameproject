#pragma once
#include "Geometry.h"

class Camera;

class Object
{
private:
public:
	Object();
	~Object();
	Rect _rect;
	Position2 _pos;
	CHAR_ST _state;
	virtual Rect& GetRect();		//‚»‚ê‚¼‚ê‚Ì‹éŒ`‚ğ•Ô‚·
	virtual Position2& GetPos();	//êŠ‚ğ•Ô‚·
	CHAR_ST GetcharState();		//ó‘Ô‚ğ•Ô‚·

	virtual void Updata();
};

