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

//�Ăяo�������̋�`��Ԃ�
Rect& Object::GetRect() 
{
	return _rect;
}

void Object::Updata() 
{

}

//���ꂼ���pos��Ԃ�
Position2& Object::GetPos()
{
	return _pos;
}
//��ڲ԰�̏�Ԃ�Ԃ�
CHAR_ST Object::GetcharState() 
{
	return _state;
}
