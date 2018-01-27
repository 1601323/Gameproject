#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Input.h"
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
	_pos.x = 350;
	_pos.y = 150;
	//mid2の場所を受け取る
	_midPos.x = 600;
	_midPos.y = 480;
	initPos = _pos;
	tmpDir = DIR_RIGHT;
	//大きさについてはとりあえずチップの大きさと一緒にしておく
	_hitRect.w = 32;
	_hitRect.h = 32;
	//中間点の大きさについて
	_midRect.w = 32;
	_midRect.h = 32;

	uiFlag = false;
	cnt = 0;
	bubbleFlag = false;
	bubble = 0;
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/金庫/中型金庫75.pmd", SCENE_RESULT);
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

void Midpoint::Updata(Input* input)
{
	_key = input->GetInput(1).key;
	_lastKey = input->GetLastKey();
	_inpInfo = input->GetInput(1);
	GetPoint();
	if (GetFlag == true||checkpointFlag == true) {
		FollowDir();
	}
}
//リトライ処理のために用意
void Midpoint::Updata()
{
	GetPoint();
	if (GetFlag == true || checkpointFlag == true) {
		FollowDir();
	}
}
void Midpoint::GetPoint()
{
	//ﾌﾟﾚｲﾔｰがたどり着いたとき（あたり判定?）
	if (_hit->IsHit(GetRect(), _pl->GetRect())) {
		checkpointFlag = true;
	}
	//目的物取得条件
	if (checkpointFlag == true && _hit->IsHit(GetRect2(), _pl->GetRect())) {
		if (GetFlag == false && checkpointFlag == true) {
			uiFlag = true;
		}
		if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
			GetFlag = true;
			uiFlag = false;
			initPos = _midPos;
		}
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
	if (GetFlag == true) {

		_midPos.x = _pl->GetPos().x;
		_midPos.y = _pl->GetPos().y;
		if (tmpDir == DIR_RIGHT) {
			_midPos.x = _pl->GetPos().x - 45;
			_midPos.y = _pl->GetPos().y - bubble;
		}
		else if (tmpDir == DIR_LEFT) {
			_midPos.x = _pl->GetPos().x + 45;
			_midPos.y = _pl->GetPos().y - bubble;
		}
	}
	else if (checkpointFlag == true) {

		_pos.x = _pl->GetPos().x;
		_pos.y = _pl->GetPos().y;
		if (tmpDir == DIR_RIGHT) {
			_pos.x = _pl->GetPos().x - 45;
			_pos.y = _pl->GetPos().y - bubble;
		}
		else if (tmpDir == DIR_LEFT) {
			_pos.x = _pl->GetPos().x + 45;
			_pos.y = _pl->GetPos().y - bubble;
		}
	}
}
void Midpoint::Draw(Position2 offset)
{
	MV1SetPosition(modelhandle, VGet(_pos.x - offset.x + (_hitRect.w / 2),SCREEN_SIZE_Y - _pos.y + offset.y - (_hitRect.h), 0));
	MV1SetRotationXYZ(modelhandle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetScale(modelhandle, VGet(2.f, 2.f, 2.f));
	MV1DrawModel(modelhandle);
	_modelmgr->SetMaterialDotLine(modelhandle, 0.1f);

	if (checkpointFlag == false && GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2), _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(210, 140, 44), true);
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
		MV1DrawModel(modelhandle);

#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}
	else if (checkpointFlag == true && GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2) , _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(0, 240, 44), true);
		//よく考えたら当たり判定取得した後いらないよね
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
		MV1DrawModel(modelhandle);

#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}
	if (GetFlag == false) {
		DrawBox(_midPos.x - offset.x, _midPos.y - offset.y, _midPos.x + _midRect.w - offset.x, _midPos.y + _midRect.h - offset.y, 0xcccccc, true);
		_midRect.SetCenter(_midPos.x+(_midRect.w/2),_midPos.y+(_midRect.h /2));
	}
	else if(GetFlag == true ){
		DrawCircle(_midPos.x - offset.x + (_midRect.w / 2) , _midPos.y - offset.y + (_midRect.h / 2), 12, GetColor(0, 240, 44), true);
	}
	if (uiFlag == true) {
		DrawString(_midPos.x-offset.x,_midPos.y-offset.y-20,"＼B!!／",0x00ff00);
	}
	_midRect.Draw(offset);
}
bool Midpoint::ReturnGetFlag()
{
	return GetFlag;
}
bool Midpoint::ReturnCheckFlag()
{
	return checkpointFlag;
}
Rect& Midpoint::GetRect()
{
	return _hitRect;
}
Rect& Midpoint::GetRect2()
{
	return _midRect;
}
Position2& Midpoint:: GetInitPos()
{
	return initPos;
}
void Midpoint::SetInitPos(CHIP_TYPE c,Position2 p)
{
	if (c == CHIP_MID_KEY) {
		_pos = p;
		initPos = _pos;
	}
	else if (c == CHIP_MID_SAFE) {
		_midPos = p;
	}
}