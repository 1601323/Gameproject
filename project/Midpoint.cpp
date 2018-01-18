#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Player.h"
#include "HitClass.h"
#include "GameMain.h"
#include "Midpoint.h"
#include "ModelMgr.h"


Midpoint::Midpoint()
{
	_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();
	GetFlag = false;
	//マップから受け取るようにする
	_pos.x = 400;
	_pos.y = 100;
	initPos = _pos;
	tmpDir = DIR_RIGHT;
	//大きさについてはとりあえずチップの大きさと一緒にしておく
	_hitRect.w = 32;
	_hitRect.h = 32;
	cnt = 0;
	bubbleFlag = false;
	bubble = 0;
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/鍵/鍵2-10.pmx", SCENE_RESULT);
}
void Midpoint::GetClass(Player* p)
{
	_pl = p;
}
Midpoint::~Midpoint()
{
	delete _hit;
	_modelmgr->ModelIdAllDelete();
}

void Midpoint::Updata()
{
	GetPoint();
	if (GetFlag == true) {
		FollowDir();
	}
}
void Midpoint::GetPoint()
{
	//ﾌﾟﾚｲﾔｰがたどり着いたとき（あたり判定?）
	if (_hit->IsHit(GetRect(), _pl->GetRect())) {
		GetFlag = true;
	}

}
void Midpoint::FollowDir()
{
	cnt++;
	if (cnt > 5) {
		cnt = 0;
		if (bubbleFlag == false) {
			bubble++;
		}
		else if (bubbleFlag == true) {
			bubble--;
		}
	}
	if (bubble > 10) {
		bubbleFlag = true;
	}
	else if (bubble < -1) {
		bubbleFlag = false;
	}
	//プレイヤーの後ろについていく処理
	if (_pl->GetDir() == DIR_RIGHT || _pl->GetDir() == DIR_LEFT) {
		tmpDir = _pl->GetDir();
	}
	_pos.x = _pl->GetPos().x;
	_pos.y = _pl->GetPos().y;
	if (tmpDir == DIR_RIGHT) {
		_pos.x = _pl->GetPos().x -30;
		_pos.y = _pl->GetPos().y - bubble;
	}
	else if (tmpDir == DIR_LEFT) {
		_pos.x = _pl->GetPos().x + 30;
		_pos.y = _pl->GetPos().y -bubble;
	}

}
void Midpoint::Draw(Position2 offset)
{
	MV1SetPosition(modelhandle, VGet(_pos.x - offset.x + (_hitRect.w / 2),SCREEN_SIZE_HEIGHT - _pos.y - (_hitRect.h), 0));
	MV1SetScale(modelhandle, VGet(0.2f, 0.2f, 0.2f));
	MV1DrawModel(modelhandle);
	_modelmgr->SetMaterialDotLine(modelhandle, 0.2f);

	if (GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2), _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(210, 140, 44), true);
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}
	else if (GetFlag == true) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2) , _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(0, 240, 44), true);
		//よく考えたら当たり判定取得した後いらないよね
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}

}
bool Midpoint::ReturnGetFlag()
{
	return GetFlag;
}

Rect& Midpoint::GetRect()
{
	return _hitRect;
}
Position2& Midpoint:: GetInitPos()
{
	return initPos;
}