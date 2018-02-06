
#include <DxLib.h>
#include <math.h>
#include <iostream>
#include "Assert.h"
#include "Geometry.h"
#include "EnemyServer.h"
#include "EmAround.h"
#include "MapCtl.h"
#include "HitClass.h"
#include "Player.h"
#include "Rope.h"
#include "ModelMgr.h"
#include "GameMain.h"
#include "ModelMgr.h"
#include "ImageMgr.h"

#include "Math.h"

EmAround::EmAround(Position2 pos,Player& pl,Rope& rope,EnemyServer& server,HitClass& hit):_pl(pl),_rope(rope),_server(server),_hit(hit)
{
	_map = MapCtl::GetInstance();
	_modelmgr = ModelMgr::Instance();

	_emRect.w = 30;
	_emRect.h = 60;	
	_pos.x = pos.x;
	_pos.y = pos.y -30;
	_initPos = _pos;

	_emEye.pos.x = _pos.x;
	_emEye.pos.y = _pos.y + (_emRect.h / 4);
	_emEye.r = 40;
	_state = EM_ST_MOVE;

	vx = 0.0f;
	vy = 0.0f;

	_dir = DIR_RIGHT;
	speed = 1;				//初期スピード設定
	moveFlag = false;
	ModelDirChangeFlag = false;

	dis = 0;
	interCnt = 0;
	fearCnt = 180;
	loseSightCnt = 180;
	//個体データ初期化
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData.midFlag = false;
	_individualData._level = ALERT_LEVEL_1;
	_rangeLevel = RANGE_1;
	midFlag = false;
	//モデル読み込み
	modelhandle = _modelmgr->ModelIdReturn("Enemy_model/teki.pmx", SCENE_RESULT);
	textureIndex = MV1GetMaterialDifMapTexture(modelhandle, 0);
	//初期角度
	modelDirAngle = AngleRad(-90.0f);
}


EmAround::~EmAround()
{
	_modelmgr->ModelIdAllDelete();
}

void EmAround::Updata()
{
	SetRange();
	_emData.lookRange = _emEye;
	_emData.lookAngle = 60;
	_emData.lookDir = _dir;
	_individualData.midFlag = _server.SendMidFlag();
	Gravity();
	Visibility();
	Move();
	//_pos.x += vx;
}
//いい処理が浮かばなかったのでここでmoveの管理させています
void EmAround::Move()
{	
	GameMain& gm = GameMain::Instance();
	vx = 0;
	if (midFlag == false) {
		if (gm.GetResultData().midFlag == true) {
			midFlag = true;
		}
	}
	//通常状態の場合
	if (_state != EM_ST_FEAR&&_individualData._level == ALERT_LEVEL_1) {
		//_state = EM_ST_MOVE;
	}
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {
		if (moveFlag == false) {
			BasicMove();		//左右へ動きます
		}
		else if (moveFlag == true) {
			InterMove();		//しばらく立ち止まる
		}
		else {
		}
	}
	//ﾌﾟﾚｲﾔｰを見つけた時
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_individualData.plFoundFlag == true) {
			FoundMove();
		}
		else if (_individualData.dataSendFlag == true) {
			LoseSight();
		}
	}
	//ﾌﾟﾚｲﾔｰを見失ったとき
	//if(_state == EM_ST_DIS )
	//怯み状態の時
	if (_state == EM_ST_FEAR) {
		moveFear();
	}
	CheckMove();
	_pos.x += vx;
}
void EmAround::BasicMove()
{
	//確認のために一時コメントアウト
	//最初はとりあえずどれくらいか動いたら反対側に動くようにする
	//dis++;
	//if (dis % 70 ==0) {
	//	//lookBackFlag = !lookBackFlag;
	//	moveFlag = true;
	//}

	//if (_state == EM_ST_MOVE) {
	//	speed = 1;
	//}
	//else if (_state == EM_ST_RETURN) {
	//	speed = 2;
	//}
	//else { speed = 1; }
	speed = midFlag ? 2 : 1;
	if (_dir == DIR_RIGHT) {		//右
		_pos.x += speed;
	}
	else if (_dir == DIR_LEFT) {	//左
		_pos.x -= speed;
	}
}
//振り返る前の動作について
void EmAround::InterMove()
{
	if (_state == EM_ST_MOVE) {	//ﾌﾟﾚｲﾔｰが発見されていない、かつクリア条件が満たされていないとき
		interCnt++;
		ModelDirChangeFlag = true;
		//とりあえず１.5秒間停止させて反対側に移動させる

		//ぐいーんと振り向いてます
		if (_dir == DIR_RIGHT) {
			modelDirAngle = AngleRad(-90.f + interCnt*-2);
		}
		else if (_dir == DIR_LEFT) {
			modelDirAngle = AngleRad(90.f - interCnt*2);
		}

		if (interCnt > 90) {
			moveFlag = false;
			ModelDirChangeFlag = false;
			interCnt = 0;
			dis = 0;


			if (_dir == DIR_RIGHT) {
				modelDirAngle = AngleRad(90.0f);
				_dir = DIR_LEFT;
			}
			else if(_dir == DIR_LEFT){
				modelDirAngle = AngleRad(-90.0f);
				_dir = DIR_RIGHT;
			}
			else {
				Assert(__FILE__, __LINE__);
			}
		}
	}
}
//ﾌﾟﾚｲﾔｰ発見時の動きについて
void EmAround::FoundMove()
{
	//現段階では視界に入っているときだけ追いかける
	speed = midFlag ? 3 : 2;
	//ﾌﾟﾚｲﾔｰのほうが右にいたら
	if (_pl.GetPos().x >= _pos.x) {
		vx += speed;
	}
	else {
		vx -= speed;
	}
}
//マップとのあたり判定にぶつかっていないか
void EmAround::CheckMove()
{
	moveFlag = false;
	//左右の移動を判定する
	Position2 nextLeftPos;		//自身の左側を判定する
	nextLeftPos.x = _pos.x - speed;	
	nextLeftPos.y = _pos.y + (_emRect.h / 2);
	Position2 nextRightPos;
	nextRightPos.x = _pos.x + (_emRect.w) + speed;
	nextRightPos.y = _pos.y + (_emRect.h/2);
	//視界で判定もおこなう
	Position2  LeftViewPos;
	LeftViewPos.x = _pos.x - speed - (_emEye.r / 2);
	LeftViewPos.y = _pos.y + (_emRect.h/2);
	Position2 RightViewPos;
	RightViewPos.x = _pos.x + (_emRect.w) + speed + (_emEye.r / 2);
	RightViewPos.y = _pos.y + (_emRect.h/2);
	//左右地面の判定を行う
	Position2 nextLeftDown;
	nextLeftDown.x = _pos.x - speed;
	nextLeftDown.y = _pos.y + (_emRect.h);
	Position2 nextRightDown;
	nextRightDown.x = _pos.x + (_emRect.w) + speed;
	nextRightDown.y = _pos.y + (_emRect.h);

	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN){	//未発見状態であれば
		if (_dir == DIR_LEFT) {		//左側に動いているとき
			if (_map->GetChipType(nextLeftPos) == CHIP_N_CLIMB_WALL ||
				_map->GetChipType(nextLeftPos) == CHIP_CLIMB_WALL	||
				_map->GetChipType(LeftViewPos) == CHIP_CLIMB_WALL	||
				_map->GetChipType(LeftViewPos) == CHIP_N_CLIMB_WALL ||
				(_hit.GimmickHit(nextLeftPos) && _hit.GimmickHitType(nextLeftPos) == GIM_ATTRACT)) {	//左が壁であれば
				moveFlag = true;	//向きを反転させる	
			}
			else if (_map->GetChipType(nextLeftDown) != CHIP_N_CLIMB_WALL &&
				_map->GetChipType(nextLeftDown) != CHIP_CLIMB_WALL) {	//左側の足元を判定する
				moveFlag = true;
			}
			else {	//条件をすり抜けてしまったとき
			}
		}
		else {		//右側に動いているとき
			if (_map->GetChipType(nextRightPos) == CHIP_N_CLIMB_WALL ||
				_map->GetChipType(nextRightPos) == CHIP_CLIMB_WALL	 ||
				_map->GetChipType(RightViewPos) == CHIP_CLIMB_WALL	 ||
				_map->GetChipType(RightViewPos) == CHIP_N_CLIMB_WALL ||
				(_hit.GimmickHit(nextRightPos) && _hit.GimmickHitType(nextRightPos) == GIM_ATTRACT)) {	//右が壁であれば												
				moveFlag = true;	//向きを反転させる
			}
			else if (_map->GetChipType(nextRightDown) != CHIP_N_CLIMB_WALL &&
				_map->GetChipType(nextRightDown) != CHIP_CLIMB_WALL) {
				moveFlag = true;
			}
			else {	//条件をすり抜けてしまったとき
			}
		}
	}
	//壁とのあたり判定
	if (_state == EM_ST_RE_DIS || _state == EM_ST_DIS) {
		Position2 nextMove[2];
		//左側
		nextMove[0].x = _pos.x + vx;
		nextMove[0].y = _pos.y + (_emRect.h / 2);
		//右側
		nextMove[1].x = _pos.x + _emRect.w + vx;
		nextMove[1].y = _pos.y + (_emRect.h / 2);
		for (int f = 0; f < 2; f++) {
			if (_map->GetChipType(nextMove[f]) == CHIP_CLIMB_WALL || _map->GetChipType(nextMove[f]) == CHIP_N_CLIMB_WALL) {
				vx = 0;
				break;
			}
		}

	}
}
//視界について
void EmAround::Visibility()
{
	_emData.lookAngle = 60;
	_emData.lookDir = _dir;
	_emData.lookRange = _emEye;
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {
		if (_hit.EnemyViewing(_emData, _pl.GetRect()) && _pl.GetStateVanish() == false && _pl.GetcharState() != ST_FEVER) {
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_ALERT || _state == EM_ST_RE_ALERT) {
		if (_hit.EnemyViewing(_emData, _pl.GetRect()) && _pl.GetStateVanish() == false && _pl.GetcharState() != ST_FEVER) {
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_hit.EnemyViewing(_emData, _pl.GetRect()) && _pl.GetStateVanish() == false && _pl.GetcharState() != ST_FEVER) {
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			_individualData.plFoundFlag = false;
			_individualData.dataSendFlag = true;
		}
	}
}
//ﾌﾟﾚｲﾔｰを見失ったときの動き
void EmAround::LoseSight()
{
	//きょろきょろすると見失った感じがすると思う
	//通常状態に戻る前にEnemyに見つけたフラグを送る
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		loseSightCnt--;
		moveFlag = true;
		if (loseSightCnt < 0) {
			_state = EM_ST_MOVE;
			loseSightCnt = 180;
			_individualData.plFoundFlag = true;
			_server.GetInfo(_individualData);
			_individualData.plFoundFlag = false;
			_individualData.dataSendFlag = false;
		}
	}
}
void EmAround::EnemyFalter()
{
	if (_state != EM_ST_FEAR) {
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&
			(_hit.IsHit(GetRect(), _rope.GetCircle()) || (_hit.IsHit(GetRect(), _rope.GetCircle2())))) {
#ifdef _DEBUG
			//DrawString(100, 100, "敵に当たったよ！", 0xffffff);
#endif
			_state = EM_ST_FEAR;
		}
		else {
		}
	}
}
//ﾛｰﾌﾟに当たった時の処理
void EmAround::moveFear()
{
	if (_state == EM_ST_FEAR) {
		fearCnt--;
		if (fearCnt <= 0) {
			_state = EM_ST_MOVE;
			fearCnt = 180;
		}
	}
}
//重力について
void EmAround::Gravity()
{
	Position2 nextPosDown[3];
	//右
	nextPosDown[0].x = _pos.x + (_emRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_emRect.h);
	//左
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_emRect.h);
	//中心
	nextPosDown[2].x = _pos.x + (_emRect.w / 2);
	nextPosDown[2].y = _pos.y + (vy / 2) + (_emRect.h);
	for (int f = 0; f < 3; f++) {
		if (_map->GetChipType(nextPosDown[f]) == CHIP_N_CLIMB_WALL ||
			_map->GetChipType(nextPosDown[f]) == CHIP_CLIMB_WALL) {
			vy = 0.0f;
			break;
		}
		else {
			vy += GRAVITY / 3.0f;
		}
	}
	_pos.y += (int)vy;
}
void EmAround::Draw(Position2 offset)
{
	ImageMgr& im = ImageMgr::Instance();
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.0f, modelDirAngle, 0.0f));
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_emRect.w / 2),_pos.y - offset.y + (_emRect.h),0)));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle,VGet(3.f,3.f,3.f));
	//テクスチャを変更
	MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("Enemy_model/teki-2.png", SCENE_RESULT), FALSE);
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(modelhandle,0.0f);

	if (_state != EM_ST_FEAR) {
		//DrawBox(_pos.x - offset.x, _pos.y - offset.y, _pos.x - offset.x + _emRect.w, _pos.y - offset.y + _emRect.h, 0x2112ff, true);
	}
	else {
		//DrawBox(_pos.x - offset.x, _pos.y - offset.y, _pos.x - offset.x + _emRect.w, _pos.y - offset.y + _emRect.h, 0x00ff00, true);
	}
	_emRect.SetCenter(_pos.x  + (_emRect.w / 2), _pos.y + (_emRect.h / 2));
	if (_state != EM_ST_FEAR) {
		if (!ModelDirChangeFlag)
		{
			if (_dir == DIR_LEFT) {
				_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
				DrawCircleGauge(_emEye.Center().x - offset.x, _emEye.Center().y - offset.y, 83.3, vigiImage[_rangeLevel], 66.6);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else if (_dir == DIR_RIGHT) {
				_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
				DrawCircleGauge(_emEye.Center().x - offset.x, _emEye.Center().y - offset.y, 33.3, vigiImage[_rangeLevel], 16.6);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	tmpPos = offset;
#ifdef _DEBUG
	//_emRect.Draw(offset);
	//_emEye.Draw(offset);
#endif

}
void EmAround::SetRange()
{
	//サイズは仮
	_individualData._level = _server.AlertLevel();
	if (midFlag == false) {
		if (_individualData._level == ALERT_LEVEL_1) {
			_rangeLevel = RANGE_1;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_2;
			}
		}
		else if (_individualData._level == ALERT_LEVEL_2) {
			_rangeLevel = RANGE_2;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_3;
			}
		}
		else if (_individualData._level == ALERT_LEVEL_3) {
			_rangeLevel = RANGE_3;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_4;
			}
		}
		else {
			_emEye.r = 60;
			_rangeLevel = RANGE_1;
		}
	}
	else {
		if (_individualData._level == ALERT_LEVEL_1) {
			_rangeLevel = RANGE_2;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_3;
			}
		}
		else if (_individualData._level == ALERT_LEVEL_2) {
			_rangeLevel = RANGE_3;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_4;
			}
		}
		else if (_individualData._level == ALERT_LEVEL_3) {
			_rangeLevel = RANGE_4;
			if (_state == EM_ST_DIS)
			{
				_rangeLevel = RANGE_5;
			}
		}
		else {
			_emEye.r = 80;
			_rangeLevel = RANGE_2;
		}
	}
	switch (_rangeLevel) {
	case RANGE_1:
		_emEye.r = 60;
		break;
	case RANGE_2:
		_emEye.r = 80;
		break;
	case RANGE_3:
		_emEye.r = 100;
		break;
	case RANGE_4:
		_emEye.r = 120;
		break;
	case RANGE_5:
		_emEye.r = 140;
		break;
	default:
		ASSERT();
	}
}
void EmAround::GetClass(HitClass* hit, Player& pl)
{
	//_hit = hit;
	_pl = pl;
}
Rect& EmAround::GetRect()
{
	return _emRect;
}
ENEMY_STATE& EmAround::GetState()
{
	return _state;
}
void EmAround::SetInitPos()
{
	_pos = _initPos;
	_state = EM_ST_MOVE;
	loseSightCnt = 180;

	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData._level = ALERT_LEVEL_1;
	_rangeLevel = RANGE_1;
}