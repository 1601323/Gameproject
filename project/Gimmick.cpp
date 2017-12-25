#include "Gimmick.h"
#include "Object.h"
#include "HitClass.h"
#include "Player.h"
#include "Input.h"


Gimmick::Gimmick( )
{
	_state = GM_NONE;
	_clearData.clearFlag = false;
}


Gimmick::~Gimmick()
{
}


//今の状態を返す
GimmickState Gimmick::nowState() 
{
	return _state;
}
//ギミックの種類を返す
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
//とりあえずあたり判定用の変数を返す
CLEAR_DATA& Gimmick::GetClearData()
{
	return _clearData;
}
void Gimmick::Draw(Position2 offset)
{

}