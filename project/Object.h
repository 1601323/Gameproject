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
	virtual Rect& GetRect();		//���ꂼ��̋�`��Ԃ�
	virtual Position2& GetPos();	//�ꏊ��Ԃ�
	CHAR_ST GetcharState();		//��Ԃ�Ԃ�

	virtual void Updata();
};

