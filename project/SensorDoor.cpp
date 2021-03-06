#include <DxLib.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "SensorDoor.h"
#include "Geometry.h"
#include "HitClass.h"
#include "Player.h"
#include "Gimmick.h"
#include "ModelMgr.h"
#include "SoundMgr.h"
#define HEIGHT 100	//ドアの縦幅（とりあえず固定）
#define WIDTH 60	//ドアの横幅（とりあ以下略）

SensorDoor::SensorDoor(Position2 pos, Player& p):_pl(p)
{
	SoundMgr& so = SoundMgr::Instance();
	//_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();
	openFlag = false;
	//posのオフセット
	_pos.x = pos.x +(MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y +MAP_CHIP_SIZE_Y;
	_initPos.x = _pos.x;
	_initPos.y = _pos.y;
	_gmRect.w = WIDTH;
	_gmRect.h = HEIGHT;
	count = 0;
	doorCountOpen = 0.0f;
	doorCountClose = 0.0f;
	_state = GM_NONE;
	enterFlag = false;
	sensordoorMotionFlag = false;
	_gimType = GIM_DOOR;
	modelhandle = _modelmgr->ModelIdReturn("door_model/door.pmx", SCENE_RESULT);
	AttachIndex = MV1AttachAnim(modelhandle, 0, -1, false);
	AttachIndexClose = MV1AttachAnim(modelhandle,1,-1,false);
	totalTime = MV1GetAttachAnimTotalTime(modelhandle, AttachIndex);
	totalTimeClose = MV1GetAttachAnimTotalTime(modelhandle,AttachIndexClose);
	so.SeStart("Se/door_open.mp3", SCENE_GAME);
}

SensorDoor::~SensorDoor()
{
	delete _hit;
	_modelmgr->ModelIdAllDelete();
}
void SensorDoor::Updata() 
{
	//_gmRect.SetCenter(_pos.x,_pos.y-(HEIGHT/2));
	//CheckDoMove();
	CheckHit();
	//Draw();
}

void SensorDoor::CheckDoMove()
{
	//時間があり次第、プレイヤーがドアの前で↑を押すと開いてリザルト画面に飛ばす処理を書きます。書いてくれてもいいよ
	//あれだったら。クリア条件を見たいしていないけどゴールする？みたいな文を出してもいいかもしれない

	//if (_pl.GetcharState() == ST_VANISH|| _pl.GetcharState() == ST_FEVER) {	//もしﾌﾟﾚｲﾔｰがステルス、もしくはフィーバー状態だったら
	//	_state = GM_MOVE2;
	//}
	//else {
	//	_state = GM_MOVE;
	//}
	//if (_state == GM_NONE) {		//最初の状態であれば何もしない

	//}
	//else if (_state == GM_MOVE) {	//ドアを閉める
	//	doorCountOpen++;
	//	if (doorCountOpen % 2 == 0) {
	//		if (count > 0) {
	//			count--;
	//		}
	//	}
	//}
	//else if (_state == GM_MOVE2) {	//ドアを開ける
	//	doorCountOpen++;
	//	if (doorCountOpen % 2 == 0) {
	//		if (count < WIDTH / 2) {
	//			count++;
	//		}
	//	}
	//}
	//else {							//判定に引っかからなかった時用
	//}
}

void SensorDoor::CheckHit()	//あたり判定の場所について
{
	//if (_pl.GetPos().x+(_pl.GetRect().w /2) <= _pos.x) {		//プレイヤーがドアより左側にいたら判定は右ドアにつける
	//	_gmRect.SetCenter(_pos.x +(WIDTH/4)+(count/2),_pos.y-(HEIGHT/2));
	//}
	//else if (_pl.GetPos().x + (_pl.GetRect().w / 2) >= _pos.x) {	//ﾌﾟﾚｲﾔｰがドアより右側にいたら判定は左のドアにつける
	//	_gmRect.SetCenter(_pos.x-(WIDTH/4)-(count/2),_pos.y-(HEIGHT/2));
	//}
	//else {
	//}
	//ドアのところで上を押すとはいれるようにしたほうがいいらしい
	//if (count * 2 >= _pl.GetRect().w) {
	//仕様変更に基づいて、いつでも上を押すとゴールできるようになっています
	//簡単なUIで説明をしたほうがいいかもしれない
		_clearData.midFlag = true;
//#ifdef _DEBUG
//		DrawString(100,30,"クリアできるよ",GetColor(244,244,244));
//#endif
//	}
//	else {
//		_clearData.midFlag = false;
//#ifdef _DEBUG
//		DrawString(100, 30, "クリアできないよ", GetColor(244, 244, 244));
//#endif
//
//	}
}

void SensorDoor::Draw(Position2 offset)
{
	SoundMgr& so = SoundMgr::Instance();
	GameMain& gm = GameMain::Instance();
	//外枠の表示
	DxLib::DrawBox((int)(_pos.x - offset.x - (WIDTH / 2))+5, (int)(_pos.y - offset.y - HEIGHT)+3, (int)(_pos.x - offset.x + (WIDTH / 2)-5), (int)_pos.y - offset.y-5, 0x000000, true);

	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, 0.f, 0.0f));
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x , _pos.y - offset.y,0.0f)));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(5.f, 5.3f, 5.f));

	if (sensordoorMotionFlag == false)
	{
		if (gm.GetResultData().goalFlag ==false)
		{
			doorCountOpen += 0.5f;
			MV1SetAttachAnimBlendRate(modelhandle, AttachIndexClose, 0.0f);
			MV1SetAttachAnimBlendRate(modelhandle, AttachIndex, 1.0f);
			//アニメーションをアタッチ
			MV1SetAttachAnimTime(modelhandle, AttachIndex, doorCountOpen);

			if (doorCountOpen >= totalTime)
			{
				doorCountOpen = totalTime;
				sensordoorMotionFlag = true;
				//so.SeStart("Bgm/door_open.mp3", SCENE_GAME);
			}
		}
		//クリアのときには扉があくだけ
		else if (gm.GetResultData().goalFlag ==true)
		{
			doorCountOpen += 0.3f;
			MV1SetAttachAnimBlendRate(modelhandle, AttachIndexClose, 0.0f);
			MV1SetAttachAnimBlendRate(modelhandle, AttachIndex, 1.0f);
			//アニメーションをアタッチ
			MV1SetAttachAnimTime(modelhandle, AttachIndex, doorCountOpen);

			if (doorCountOpen >= totalTime)
			{
				doorCountOpen = totalTime;
				sensordoorMotionFlag = false;
			}
		}
	}
	else if (sensordoorMotionFlag == true)
	{
		doorCountClose += 0.5f;
		MV1SetAttachAnimBlendRate(modelhandle, AttachIndex,0.0f);
		MV1SetAttachAnimBlendRate(modelhandle, AttachIndexClose, 1.0f);

		//アニメーションをアタッチ
		MV1SetAttachAnimTime(modelhandle, AttachIndexClose, doorCountClose);
		if (doorCountClose >= totalTimeClose)
		{
			doorCountClose = totalTimeClose;
			doorCountOpen = 0;
			//sensordoorMotionFlag = true;
		}
	}
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(modelhandle, 0.0f);


	//右扉
	//DxLib::DrawBox((int)(_pos.x - offset.x+count),(int)(_pos.y - offset.y - HEIGHT),(int)(_pos.x - offset.x + (WIDTH / 2)),(int)_pos.y - offset.y,0xffffffff,true);
	//左扉
	//DxLib::DrawBox((int)(_pos.x - offset.x - (WIDTH / 2)),(int)( _pos.y - offset.y - HEIGHT), (int)(_pos.x - offset.x - count ), (int)_pos.y - offset.y, 0xffffffff, true);

	_gmRect.SetCenter(_pos.x , _pos.y - (HEIGHT / 2));
#ifdef _DEBUG
	//_gmRect.Draw(offset);
#endif
}

Rect& SensorDoor::GetRect() 
{
	return _gmRect;
}

GIMMICK_TYPE& SensorDoor::GetType()
{
	return _gimType;
}
RESULT_DATA& SensorDoor::GetClearData()
{
	return _clearData;
}
void SensorDoor::SetInit()
{
	_pos = _initPos;
}