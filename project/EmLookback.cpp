#include "DxLib.h"
#include "Geometry.h"
#include "EmLookback.h"
#include "MapCtl.h"
#include "HitClass.h"
#include "EnemyServer.h"
#include "Player.h"
#include "Rope.h"



EmLookback::EmLookback(Position2 pos, Player& pl, Rope& rope, EnemyServer& server) :_player(pl), _rope(rope), _server(server)
{
	_hit = new HitClass();
	//_player = new Player();
	_map = MapCtl::GetInstance();
	_pos.x = pos.x;
	_pos.y = pos.y;
	_dir = DIR_RIGHT;
	_emRect.w = 32;
	_emRect.h = 32;
	_emEye.pos.x = _pos.x;
	_emEye.pos.y = _pos.y + (_emRect.h / 4);
	_emEye.r = 40;
	_emType = ENEMY_TURN;
	_state = EM_ST_MOVE;

	upAngle = 120;
	downAngle = 60;
	emSpeed = 1;
	LookCount = 0;
	FearCount = 180;
	loseSightCnt = 180;

	_tmpOffset.x = 0;
	_tmpOffset.y = 0;
	//個体データ初期化
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData._level = ALERT_LEVEL_1;

}

EmLookback::~EmLookback()
{
	delete _hit;
}

void EmLookback::Updata()
{
	//setDir();
	SetMove();
	Visibility();
	//LookPl();
	//moveFear();
}

void EmLookback::Draw(Position2 offset)
{
	switch (_state)
	{
	case EM_ST_NONE:
	case EM_ST_MOVE:
		DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0xff0000, true);
		break;
	case EM_ST_DIS:
		DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0x0000ff, true);
		break;
	case EM_ST_RETURN:
		break;
	case EM_ST_RE_DIS:
		break;
	case EM_ST_FEAR:
		DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0x00ff00, true);
		break;
	default:
		break;
	}
	_tmpOffset = offset;
	_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
	returnDir(offset);
	_emRect.SetCenter(_pos.x + (_emRect.w / 2), _pos.y  +(_emRect.h / 2));
	_emRect.Draw(offset);
	_emEye.Draw(offset);

#ifdef _DEBUG
	DrawFormatString(10, 380, 0xffffff, "振り返り:%d", LookCount);
#endif 
}
void EmLookback::SetMove()
{
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {
		setDir();
	}
	else if(_state == EM_ST_DIS || _state == EM_ST_RE_DIS){
		//追いかけてくる動き
		if (_individualData.plFoundFlag == true) {
			LookPl();
		}
		else if (_individualData.dataSendFlag == true) {
			LoseSight();
		}
		else{}
	}
	else if (_state == EM_ST_FEAR) {
		//怯みの動き
		moveFear();
	}
	else {
		setDir();		//状態の例外
	}
}
void EmLookback::setDir(void)
{
	if (_state == EM_ST_MOVE) {
		//3秒ごとに向きが変わる
		if (LookCount < EM_LOOKBACK_TIME) {
			LookCount++;
			if (LookCount == EM_LOOKBACK_TIME) {
				LookCount *= -1;
			}
		}
		if (LookCount < 0) {
			_dir = DIR_RIGHT;
			_emEye.SetCenter(_pos.x  + _emRect.w, _pos.y  + (_emRect.h / 4), _emEye.r);
		}
		else {
			_dir = DIR_LEFT;
			_emEye.SetCenter(_pos.x , _pos.y + (_emRect.h / 4), _emEye.r);
		}
	}
	else {
		if (_dir == DIR_RIGHT) {
			_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
		}
		else if (_dir == DIR_LEFT) {
			_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
		}
		LookCount = 0;
	}
}
void EmLookback::Visibility()
{
	_emData.lookAngle = 60;
	_emData.lookDir = _dir;
	_emData.lookRange = _emEye;
	////視界判定(プレイヤーを見つけたとき)
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {

		if (_hit->EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
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
		if (_hit->EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
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
		if (_hit->EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			_individualData.plFoundFlag = false;
			_individualData.dataSendFlag = true;
		}
	}

}
void EmLookback::LookPl(void)
{
	//プレイヤーを見つけたら追いかけてくる
	//今は向いている方向に動くようにしている
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_dir == DIR_LEFT) {
			if (_map->GetChipType({ _pos.x ,_pos.y + (_emRect.h / 2) }) == CHIP_BLANK
				|| _map->GetChipType({ _pos.x ,_pos.y + (_emRect.h / 2) }) == CHIP_ROPE_FALL) {
				_pos.x -= emSpeed;
			}
		}
		else if (_dir == DIR_RIGHT) {
			if (_map->GetChipType({ _pos.x + _emRect.w,_pos.y + (_emRect.h / 2) }) == CHIP_BLANK
				|| _map->GetChipType({ _pos.x + _emRect.w,_pos.y + (_emRect.h / 2) }) == CHIP_ROPE_FALL) {
				_pos.x += emSpeed;
			}
		}
		else {
		}
	}
}

void EmLookback::moveFear(void)
{
	if (_state == EM_ST_FEAR) {
		FearCount--;
		if (FearCount <= 0) {
			_state = EM_ST_MOVE;
			FearCount = 180;
		}
	}
#ifdef _DEBUG
	DrawFormatString(10, 450, 0xffffff, "怯み:%d", FearCount);
#endif
}
void EmLookback::EnemyFalter()
{
	if (_state != EM_ST_FEAR) {
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			_state = EM_ST_FEAR;
		}
		else {
		}
	}
}
void EmLookback::LoseSight()
{
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		loseSightCnt--;
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
Rect & EmLookback::GetRect()
{
	return _emRect;
}

ENEMY_TYPE & EmLookback::GetType()
{
	return _emType;
}
//状態を返す
ENEMY_STATE& EmLookback::GetState()
{
	return _state;
}
void EmLookback::GetClass(HitClass * hit, Player & pl)
{
	_hit = hit;
	_player = pl;
}
//オフセットの為向いている方向を確認します
void EmLookback::returnDir(Position2 offset)
{
	if (_state == EM_ST_MOVE) {
		if (LookCount < 0) {
			_dir = DIR_RIGHT;
			_emEye.SetCenter(_pos.x  + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
		}
		else {
			_dir = DIR_LEFT;
			_emEye.SetCenter(_pos.x , _pos.y  + (_emRect.h / 4), _emEye.r);
		}
	}
	else {
		if (_state != EM_ST_FEAR) {
			if (_dir == DIR_RIGHT) {
				_emEye.SetCenter(_pos.x  + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
			}
			else if (_dir == DIR_LEFT) {
				_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
			}
		}
	}
}