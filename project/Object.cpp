#include <vector>
#include <iostream>
#include "Camera.h"
#include "Object.h"
#include "Geometry.h"

using namespace std;

Object::Object()
{
}


Object::~Object()
{
}

//ŒÄ‚Ño‚µ‚½•¨‚Ì‹éŒ`‚ğ•Ô‚·
Rect& Object::GetRect() 
{
	return _rect;
}

void Object::Updata() 
{

}

//‚»‚ê‚¼‚ê‚Ìpos‚ğ•Ô‚·
Position2& Object::GetPos()
{
	return _pos;
}
//ÌßÚ²Ô°‚Ìó‘Ô‚ğ•Ô‚·
CHAR_ST Object::GetcharState() 
{
	return _state;
}
