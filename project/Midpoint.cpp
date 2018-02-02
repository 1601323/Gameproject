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
	_modelPos = _midPos;
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
	alfa = 255;
	tranceMax = 50;
	//モデル読み込み
	Safehandle = _modelmgr->ModelIdReturn("gimmick_model/金庫/中型金庫75.pmd", SCENE_RESULT);
	keyhandle = _modelmgr->ModelIdReturn("gimmick_model/鍵/鍵2-10.pmx", SCENE_RESULT);
	Targethandle = _modelmgr->ModelIdReturn("gimmick_model/フラスコ/丸底フラスコ.pmx", SCENE_RESULT);
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
	//目的物のモデル
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(Targethandle, ConvWorldPosToScreenPos(VGet(_midPos.x - offset.x + (_hitRect.w / 2), _midPos.y - offset.y + (_hitRect.h), 0)));
	//モデルの拡大縮小値の設定
	MV1SetScale(Targethandle, VGet(10.f, 10.f, 10.f));

	//金庫のモデル
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(Safehandle, ConvWorldPosToScreenPos(VGet(_modelPos.x - offset.x + (_midRect.w / 2), _modelPos.y - offset.y + (_midRect.h), 0)));
	//モデルの拡大縮小値の設定
	MV1SetScale(Safehandle, VGet(2.f, 2.f, 2.f));
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(Safehandle, 0.0f);

	//鍵のモデル
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(keyhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_hitRect.w / 2),_pos.y - offset.y + (_hitRect.h), 0)));
	//モデルの拡大縮小値の設定
	MV1SetScale(keyhandle, VGet(0.2f, 0.2f, 0.2f));

	MV1SetOpacityRate(keyhandle, alfa / 255.f);
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(keyhandle, 0.5f);

	if (checkpointFlag == false && GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2), _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(210, 140, 44), true);
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
		//MV1DrawModel(keyhandle);

#ifdef _DEBUG
		//_hitRect.Draw(offset);
#endif
	}
	else if (checkpointFlag == true && GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2) , _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(0, 240, 44), true);
		//よく考えたら当たり判定取得した後いらないよね
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));

		if (_pl->GetcharState() == ST_VANISH)
		{
			alfa = max(alfa - 1, tranceMax);
		}
		else {
			alfa = 255;
		}

		MV1SetOpacityRate(keyhandle, alfa / 255.f);
		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(keyhandle, 0.5f);

#ifdef _DEBUG
		//_hitRect.Draw(offset);
#endif
	}
	if (GetFlag == false) {
		//DrawBox(_midPos.x - offset.x, _midPos.y - offset.y, _midPos.x + _midRect.w - offset.x, _midPos.y + _midRect.h - offset.y, 0xcccccc, true);
		_midRect.SetCenter(_midPos.x+(_midRect.w/2),_midPos.y+(_midRect.h /2));
		//モデルを描画
		//MV1DrawModel(Safehandle);
	}
	else if(GetFlag == true ){
		//DrawCircle(_midPos.x - offset.x + (_midRect.w / 2) , _midPos.y - offset.y + (_midRect.h / 2), 12, GetColor(0, 240, 44), true);

		if (_pl->GetcharState() == ST_VANISH)
		{
			alfa = max(alfa - 1, tranceMax);
		}
		else {
			alfa = 255;
		}
		MV1SetOpacityRate(Targethandle, alfa / 255.f);
		//モデルを描画
		_modelmgr->Draw(Targethandle, 0.1f);
	}
	if (uiFlag == true) {
	//	DrawString(_midPos.x-offset.x,_midPos.y-offset.y-20,"＼B!!／",0x00ff00);
	}
#ifdef _DEBUG
	//_midRect.Draw(offset);
#endif
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
		_modelPos = p;
	}
}