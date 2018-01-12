#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Player.h"
#include "HitClass.h"
#include "GameMain.h"
#include "Midpoint.h"



Midpoint::Midpoint()
{
	_hit = new HitClass();
	GetFlag = false;
	//マップから受け取るようにする
	_pos.x = 400;
	_pos.y = 60;
	//大きさについてはとりあえずチップの大きさと一緒にしておく
	_hitRect.w = 32;
	_hitRect.h = 32;
}
Midpoint::~Midpoint()
{
	delete _hit;
}

void Midpoint::Updata()
{

}
void Midpoint::GetPoint()
{
	//ﾌﾟﾚｲﾔｰがたどり着いたとき（あたり判定?）

}
void Midpoint::Draw(Position2 offset)
{
	if (GetFlag = false) {
		DrawCircle(_pos.x+offset.x,_pos.y +offset.y,10,GetColor(210,140,44),true);
	}
	else if (GetFlag == true) {

	}
	_hitRect.SetCenter(_pos.x+(_hitRect.w /2) , _pos.y+(_hitRect.h/2));
	_hitRect.Draw(offset);
}
bool Midpoint::ReturnGetFlag()
{
	return GetFlag;
}