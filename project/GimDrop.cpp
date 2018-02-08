#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "GimDrop.h"
#include "HitClass.h"
#include "Input.h"
#include "Rope.h"
#include "Player.h"
#include "MapCtl.h"
#include "SoundMgr.h"
#include "ModelMgr.h"


GimDrop::GimDrop(Position2 pos,Rope& r,Player& p):_rope(r),_player(p)
{
	_hit = new HitClass();
	_map = MapCtl::GetInstance();
	_modelmgr = ModelMgr::Instance();
	_state = GM_NONE;
	sensordoorMotionFlag = false;
	gravity = 0.9;
	velocity = 0;
	_gmRect.h = 30;
	_gmRect.w = 30;
	_pos.x = pos.x + (MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y+MAP_CHIP_SIZE_Y - (_gmRect.h / 2);
	_initPos.x = _pos.x;
	_initPos.y = _pos.y;
	dropFlag = false;
	count = 60;
	_fd.feverCnt = 0;
	_fd.feverTime = 0;

	_gimType = GIM_FALL;
	//モデル読み込み
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/bread/breadone.pmd", SCENE_RESULT);
}


GimDrop::~GimDrop()
{
	delete _hit;
}

void GimDrop::Updata(Input& _input)
{
	//_gmRect.SetCenter(_pos.x , _pos.y);

	_key = _input.GetInput(1).key;
	_lastKey = _input.GetLastKey();
	_inpInfo = _input.GetInput(1);
	if (_state != GM_END) {
		CheckDoMove();
	}
	if (_state == GM_HIT) {
		MoveLeft();
//		_fd.feverCnt = 1;

		SendFeverData();

	}
	else if (_state == GM_MOVE) {
		MoveRight();
	//	_fd.feverCnt = 1;
		SendFeverData();

	}
	else {

	}
	GetItem();
	_fd =GameMain::Instance().ReturnFeverData();
	//cout << _fd.feverCnt << endl;
	//Draw();	
}
//_stateの変更について
void GimDrop::CheckDoMove()
{
	//if (_state == GM_NONE ||_state== GM_PAUSE) {	//状態が一度しか切り替わらないようにする
	if(_state != GM_HIT && _state != GM_MOVE){							//動いている途中でも当てたら方向転換ができるようになってます
		////ロープとのあたり判定を取る
		if (_rope.GetRopeState()== ST_ROPE_SHRINKING &&
			((_hit->IsHit(GetRect(), _rope.GetCircle())) || (_hit->IsHit(GetRect(), _rope.GetCircle2())))) {
			//if (_rope.GetCircle().pos.x < _pos.x) {	//ropeのﾚｸﾄのXが物体のXより大きい
				_state = GM_HIT;		//左側に動く
			//}
			//else {
			//	_state = GM_MOVE;		//右側に動く
			//}
		}	
		//デバック用　ｴﾝﾀｰで作動（移動量を設定しないと頭おかしい挙動をします）
		//if (_inpInfo.key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
		//	//動き始める
		//	_state = GM_HIT;
		//}
	}
}

void GimDrop::MoveLeft()
{

	//ﾛｰﾌﾟの移動量を受け取る
	_pos.x += _rope.GetRopeVec().x;
	_pos.y -= _rope.GetRopeVec().y;

	if (_rope.GetRopeVec().x == 0) {
		_state = GM_PAUSE;
	}
}

void GimDrop::MoveRight()
{
	//ﾛｰﾌﾟの移動量を受け取る
	_pos.x += _rope.GetRopeVec().x;
	_pos.y -= _rope.GetRopeVec().y;
	if (_rope.GetRopeVec().x == 0) {
		_state = GM_PAUSE;
	}
}
void GimDrop::GetItem()
{
	SoundMgr& so = SoundMgr::Instance();
	if (_hit->IsHit(_gmRect, _player.GetRect())) {
		if (_state != GM_END) {
			_state = GM_END;
			_fd.feverCnt++;
			so.SeStart("Se/suck.mp3",SCENE_RESULT);
			GameMain::Instance().SetFeverData(_fd);
		}
	}
}
//描画
void GimDrop::Draw(Position2 offset) 
{
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(AngleRad(90.0f), 0.0f, 0.0f));
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x, _pos.y - offset.y, 0)));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(15.0f, 15.0f, 15.0f));
	if (_state != GM_END && _state != GM_PAUSE) {
		//そのものの描画
		//DrawCircle(_pos.x - offset.x,_pos.y -offset.y,10,GetColor(255,0,255),true);

		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(modelhandle, 0.0f);
	}
	else if (_state == GM_END) {
		//割れたりしてる描画
		count--;
		if (count >= 0) {
			//DrawString(_pos.x - offset.x -70, _pos.y - offset.y -30, "＼ガシャーン／", 0xffff00);
		}
		//_modelmgr->Draw(modelhandle, 0.0f);
		//DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(255, 0, 0), true);
	}
	else if (_state == GM_PAUSE) {		//移動が一時停止しているとき（壁にぶつかったなど)
		//DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(0, 0, 255), true);
		_modelmgr->Draw(modelhandle, 0.0f);
	}
	_gmRect.SetCenter(_pos.x , _pos.y);
#ifdef _DEBUG
	//_gmRect.Draw(offset);
#endif
}
//あたり判定用
Rect& GimDrop::GetRect()
{
	return _gmRect;
}
//gimmickの種類を返すための関数
GIMMICK_TYPE& GimDrop::GetType()
{
	return _gimType;
}

void GimDrop::SendFeverData()
{
	GameMain::Instance().SetFeverData(_fd);
}
void GimDrop::SetInit()
{
	_pos = _initPos;
	_state = GM_NONE;
}