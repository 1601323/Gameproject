#include <DxLib.h>
#include <math.h>
#include "Assert.h"
#include "Geometry.h"
#include "EnemyServer.h"
#include "EmAround.h"
#include "MapCtl.h"
#include "HitClass.h"
#include "Player.h"
#include "Rope.h"

#include "Math.h"

EmAround::EmAround(Position2 pos,Player& pl,Rope& rope,EnemyServer& server):_pl(pl),_rope(rope),_server(server)
{
	_map = MapCtl::GetInstance();
	//_pl = new Player();
	_hit = new HitClass();
	_pos.x = pos.x;
	_pos.y = pos.y;

	_emRect.w = 30;
	_emRect.h = 30;	
	_emEye.pos.x = _pos.x;
	_emEye.pos.y = _pos.y + (_emRect.h / 4);
	_emEye.r = 40;
	_state = EM_ST_MOVE;

	vx = 0.0f;
	vy = 0.0f;

	_dir = DIR_RIGHT;
	speed = 1;				//初期スピード設定
	moveFlag = false;

	dis = 0;
	interCnt = 0;
	fearCnt = 180;
	loseSightCnt = 180;
	//個体データ初期化
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData._level = ALERT_LEVEL_1;
}


EmAround::~EmAround()
{
	delete _hit;
}

void EmAround::Updata()
{
	_emData.lookRange = _emEye;
	_emData.lookAngle = 60;
	Gravity();
	Visibility();
	Move();
}
//いい処理が浮かばなかったのでここでmoveの管理させています
void EmAround::Move()
{	
	CheckMove();
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
		//とりあえず１秒間停止させて反対側に移動させる
		if (interCnt > 60) {
			moveFlag = false;
			interCnt = 0;
			dis = 0;
			if (_dir == DIR_RIGHT) {
				_dir = DIR_LEFT;
			}
			else if(_dir == DIR_LEFT){
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

	//ﾌﾟﾚｲﾔｰのほうが右にいたら
	if (_pl.GetPos().x >= _pos.x) {
		_pos.x += speed;
	}
	else {
		_pos.x -= speed;
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
				_map->GetChipType(nextLeftPos) == CHIP_CLIMB_WALL ) {	//左が壁であれば
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
				_map->GetChipType(nextRightPos) == CHIP_CLIMB_WALL) {	//右が壁であれば												
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
}
//視界について
void EmAround::Visibility()
{
	_emData.lookAngle = 60;
	_emData.lookDir = _dir;
	_emData.lookRange = _emEye;
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {

		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "発見", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "索敵中", 0xffffff);
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_ALERT || _state == EM_ST_RE_ALERT) {
		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "発見", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "索敵中", 0xffffff);
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "発見", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "索敵中", 0xffffff);
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
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			DrawString(100, 100, "敵に当たったよ！", 0xffffff);
			_state = EM_ST_FEAR;
		}
		else {
			DrawString(100, 100, "うぇい", 0xffffff);
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
	if (_state != EM_ST_FEAR) {
		DrawBox(_pos.x - offset.x, _pos.y - offset.y, _pos.x - offset.x + _emRect.w, _pos.y - offset.y + _emRect.h, 0x2112ff, true);
	}
	else {
		DrawBox(_pos.x - offset.x, _pos.y - offset.y, _pos.x - offset.x + _emRect.w, _pos.y - offset.y + _emRect.h, 0x00ff00, true);
	}
	_emRect.SetCenter(_pos.x - offset.x + (_emRect.w / 2), _pos.y - offset.y + (_emRect.h / 2));
	if (_dir == DIR_LEFT) {
		_emEye.SetCenter(_pos.x - offset.x, _pos.y - offset.y + (_emRect.h / 4), _emEye.r);
	}
	else if (_dir == DIR_RIGHT) {
		_emEye.SetCenter(_pos.x - offset.x + _emRect.w, _pos.y - offset.y + (_emRect.h / 4), _emEye.r);
	}
	_emRect.Draw();
	_emEye.Draw();
}

void EmAround::GetClass(HitClass* hit, Player& pl)
{
	_hit = hit;
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