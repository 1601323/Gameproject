#include "Gimmick.h"
#include "Object.h"
#include "HitClass.h"
#include "Player.h"
#include "Input.h"


Gimmick::Gimmick( )
{
	_state = GM_NONE;
	_clearData.midFlag = false;
	sensordoorMotionFlag = false;
}


Gimmick::~Gimmick()
{
}

Rect& Gimmick::GetRect()
{
	return _gmRect;
}
//���̏�Ԃ�Ԃ�
GimmickState Gimmick::nowState() 
{
	return _state;
}
//�M�~�b�N�̎�ނ�Ԃ�
GIMMICK_TYPE& Gimmick::GetType()
{
	return _gimType;
}
void Gimmick::Updata(Input& input) 
{

}
void Gimmick::Updata() 
{

}
//�Ƃ肠���������蔻��p�̕ϐ���Ԃ�
RESULT_DATA& Gimmick::GetClearData()
{
	return _clearData;
}
void Gimmick::Draw(Position2 offset)
{

}