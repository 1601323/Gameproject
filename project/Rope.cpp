#include <DxLib.h>
#include <math.h>
#include <iostream>

#include "Rope.h"
#include "Player.h"
#include "Input.h"
#include "Math.h"
#include "HitClass.h"
#include "MapCtl.h"
#include "Geometry.h"

Rope::Rope() 
{
	_tmpOffset.x = 0;
	_tmpOffset.y = 0;
}

Rope::Rope(Player* player)
{
	_player = player;
	_mapctl = MapCtl::GetInstance();
	RopeInit();
}

Rope::~Rope()
{
}

void Rope::Updata(Input* input,Position2 offset)
{
	DirChange();
	(this->*_states)(input);
	_tmpOffset = offset;
	_key = input->GetInput(1).key;
	_lastkey = input->GetLastKey();
	_inputinfo = input->GetInput(1);

#ifdef _DEBUG
	DrawString(50, 120, "D:直線 横", 0xffffffff);
	DrawString(50, 140, "W:直線 上", 0xffffffff);
	DrawString(50, 160, "S:直線 下", 0xffffffff);
	DrawString(50, 180, "E:曲線 上", 0xffffffff);
	DrawString(50, 200, "R:曲線 下", 0xffffffff);

	DrawFormatString(350, 100, 0xffffff, "%f", _HitPos.x);
	DrawFormatString(350, 120, 0xffffff, "%f", _HitPos.y);

	DrawString(450, 340, "速度", 0xffffffff);
	DrawFormatString(450, 360, 0xffffff, "%f", _vec.x);
	DrawFormatString(450, 380, 0xffffff, "%f", _vec.y);

#endif
}

//初期化処理
void Rope::RopeInit(void)
{
	_states     = &Rope::Ready;
	_state      = ST_ROPE_READY;
	_ropeDir    = ROPE_DIR_NON;
	_curveDir   = ROPE_DIR_CURVE_NON;
	_minValue   = SV_MID;

	_RopeRect.h = ROPE_RECT_H;
	_RopeRect.w = ROPE_RECT_W;
	range       = ROPE_RANGE;
	theta       = ROPE_THETA;
	omega       = ROPE_OMEGA;

	RopeTurnFlag = false;
	dirFlag      = false;
	padFlag      = false;

	RotationPos = { 0,-20 };
	_vec = { 0.f,0.f };
	rote = 0.0f;

	//0～19 の20
	for (int j = 0; j < ROPE_LENGTH_MAX; j++)
	{
		ropeinfo.push_back(j);//先頭から入れる
	}
}

//ロープ描画処理
void Rope::DrawRopeRect(void)
{
	//rect
	/*RopeRect.SetCenter(_rope[*itr].x + (RopeRect.w / 2),
		_rope[*itr].y + (RopeRect.h / 2));
	RopeRect.Draw();*/

	//circle
	_RopeCircle.SetCenter(_rope[*itr-1].x + (_RopeRect.w / 2) - _tmpOffset.x,
		_rope[*itr-1].y + (_RopeRect.h / 2)- _tmpOffset.y, range);

	if (RopeTurnFlag)
	{
		DrawLineSet(_rope[0], _rope[*itr - 1], 0xfffffff);
	}
	else 
	{
		if (*itr != ROPE_LENGTH_MAX - 1)//軌道線を描画
		{
			DrawLineSet(_rope[*itr], _rope[*itr + 1], 0xfffffff);
		}
	}

	_RopeCircle.Draw();
}

//DrawLineをまとめた関数 offset値の設定が分かりにくかったから
void Rope::DrawLineSet(Position2 startpos, Position2 endpos,int color)
{
	//cout<<_tmpOffset.x<<endl;
	DrawLine(startpos.x - _tmpOffset.x +(_RopeRect.w / 2), startpos.y - _tmpOffset.y + (_RopeRect.h / 2),
		endpos.x - _tmpOffset.x + (_RopeRect.w / 2),endpos.y - _tmpOffset.y + (_RopeRect.h / 2),color);
}

//ロープの移動処理(直線ver)
//SetRopeRad()の値によって角度が変わります
void Rope::SetRope(void)
{
	if (RopeTurnFlag)
	{
		_vec.x = cos(SetRopeRad()) * SetVec().x;
		_vec.y = sin(SetRopeRad()) * SetVec().y;

		_rope[*itr].x = _rope[*itr - 1].x + _vec.x;
		_rope[*itr].y = _rope[*itr - 1].y + _vec.y;
	}
	else
	{
		if (*itr != ROPE_LENGTH_MAX-1)//範囲チェック
		{
			_vec.x = cos(SetRopeRad()) * SetVec().x;
			_vec.y = sin(SetRopeRad()) * SetVec().y;

			_rope[*itr + 1].x = _rope[*itr].x + _vec.x;
			_rope[*itr + 1].y = _rope[*itr].y + _vec.y;
		}
	}
}

//ロープの移動処理(曲線ver)
void  Rope::SetCurveRope(void)
{
	ThetaSet();

	if (RopeTurnFlag)
	{
		_vec.x = cos(AngleRad(theta)) *  SetVec().x;
		_vec.y = sin(AngleRad(theta)) *  SetVec().y;

		_rope[*itr].x = _rope[*itr - 1].x + _vec.x;
		_rope[*itr].y = _rope[*itr - 1].y + _vec.y;
	}
	else
	{
		if (*itr != ROPE_LENGTH_MAX - 1)//範囲チェック
		{
			_vec.x = cos(AngleRad(theta)) *  SetVec().x;
			_vec.y = sin(AngleRad(theta)) *  SetVec().y;

			_rope[*itr + 1].x = _rope[*itr].x + _vec.x;
			_rope[*itr + 1].y = _rope[*itr].y + _vec.y;
		}
	}
}

//曲線時のposのずらし方を決めている関数
//汚いです
void Rope::ThetaSet(void)
{
	if (dirFlag)//右向き
	{
		if (_curveDir == ROPE_DIR_CURVE_UP)
		{
			theta -= omega;
		}
		else if (_curveDir == ROPE_DIR_CURVE_LOW)
		{
			theta += omega;
		}
	}
	else
	{
		if (_curveDir == ROPE_DIR_CURVE_UP)
		{
			theta += omega;
		}
		else if (_curveDir == ROPE_DIR_CURVE_LOW)
		{
			theta -= omega;
		}
	}
}

//プレイヤーの向きを見て撃つ方向をflagで決める
void Rope::DirChange(void)
{
	if (_player->GetDir() == DIR_RIGHT)
	{
		dirFlag = true;
	}
	else if (_player->GetDir() == DIR_LEFT)
	{
		dirFlag = false;
	}
	else {
	}
}

//ゲームパッドが刺さっているかどうかを見るflag
void Rope::CheckPad(void)
{
	padFlag = _inputinfo.num ? 1 : 0;
}

//ロープを発射する準備
//キーが押されたら移動処理に移る
void Rope::Ready(Input* input)
{
	if (_state == ST_ROPE_READY)
	{
		CheckPad();

		for (itr = ropeinfo.begin(); itr != last; itr++)
		{
			_rope[*itr] = _player->GetPos();
		}

		//発射準備のボタンが変わっています key A pad X
		if (_key.keybit.L_LEFT_BUTTON && !_lastkey.keybit.L_LEFT_BUTTON && !padFlag ||
			_key.keybit.X_BUTTON && !_lastkey.keybit.X_BUTTON && padFlag)
		{
			RotationPos = _player->GetPos() + RotationPos;
			RopeTurnFlag = false;
			_minValue = SV_MID;
			_HitPos = { 0,0 };
			_vec = { 0,0 };
			theta = ROPE_THETA;//回すために仮に代入しているだけです
			_state = ST_ROPE_SELECT;
			_states = &Rope::SelectDir;
		}
		else {
		}
#ifdef _DEBUG
		DrawString(200, 200, "待機中", 0xfffffff);
#endif
	}
	else {
	}
}

//ロープの移動の方向を決める処理(くるくる)
//この状態のときは、the world状態
void Rope::SelectDir(Input* input)
{
	if (_state == ST_ROPE_SELECT)
	{
		//発射前にぐるぐる回しています 仮の動きなのですごい適当
		theta -= 30;
		_RopeCircle.SetCenter(RotationPos.x - _tmpOffset.x + (_RopeRect.w / 2),
			                  RotationPos.y - _tmpOffset.y +(_RopeRect.h / 2), range);
		_RopeCircle.Draw();

		_vec.x = cos(AngleRad(theta)) *  SetVec().x;
		_vec.y = sin(AngleRad(theta)) *  SetVec().y;

		RotationPos.x = RotationPos.x + _vec.x;
		RotationPos.y = RotationPos.y + _vec.y;
		//くるくるの線
		DrawLineSet(_rope[0], RotationPos,0xffffff);


		//ロープくるくる解除 Readyの状態に戻す
		if (_key.keybit.L_LEFT_BUTTON && !_lastkey.keybit.L_LEFT_BUTTON && !padFlag ||
			_key.keybit.X_BUTTON && !_lastkey.keybit.X_BUTTON && padFlag)
		{
			_vec = { 0,0 };
			RotationPos = { 0,-20 };//-20はplayerの位置から20上で回す
			_state = ST_ROPE_READY;
			_states = &Rope::Ready;
		}

		//押したボタン or 入れた角度によって向きを決める
		//ここだけのために関数ポインタの引数にinputを設定している(怒)
		if ((RopeDirSet() || RopeDirSetPad(input)) != ROPE_DIR_NON ||
			CurveRopeDirSet() != ROPE_DIR_CURVE_NON)
		{
			RopeTurnFlag = false;
			theta = SetRopeRadCurve();
			_vec = { 0,0 };
			itr = ropeinfo.begin();
			_state = ST_ROPE_EXTENDING;
			_states = &Rope::Extending;
		}
	}
#ifdef _DEBUG
	DrawString(200, 200, "選択中", 0xfffffff);
#endif
}

//ロープの行きの処理
//ROPE_LENGTH_MAXまで伸び続ける
//最大まで行くと待ちの処理に移る
void Rope::Extending(Input* input)
{
	if (_state == ST_ROPE_EXTENDING)
	{
		if (++itr != last)
		{
			DrawRopeRect();

			if (_ropeDir != ROPE_DIR_NON)
			{
				SetRope();
			}
			else {
			}

			if (_curveDir != ROPE_DIR_CURVE_NON)
			{
				SetCurveRope();
			}
			else {
			}

			//伸ばしている最中にギミックやステージにあたれば強制的に戻す
			if (_hit->GimmickHitType(GetCircle()) || _hit->EnemyHit(GetCircle())||
				_mapctl->GetChipType(_rope[*itr+1]) == CHIP_N_CLIMB_WALL ||
				_mapctl->GetChipType(_rope[*itr+1]) == CHIP_CLIMB_WALL)
			{
				_HitPos = _rope[*itr];
				itr = last;
				_state = ST_ROPE_SHRINKING;
				RopeTurnFlag = true;
				_states = &Rope::Shrinking;
			}
			else {
			}
		}
		else
		{
			time = WAIT_TIME;
			_state = ST_ROPE_EXTENDED;
			_states = &Rope::Extended;
		}
#ifdef _DEBUG
		DrawString(200, 250, "移動中", 0xfffffff);
#endif
	}
	else {
	}
}

//timeが0になるまで待機の処理
//ここでflagや回す順番を変えてます
void Rope::Extended(Input* input)
{
	if (_state == ST_ROPE_EXTENDED)
	{
		if (time > 0) 
		{
			//その場で描画
			_RopeCircle.Draw();
			DrawLineSet(_rope[0], _rope[18], 0xfffffff);
			time--;
		}
		else {
			itr = last;
			_state = ST_ROPE_SHRINKING;
			RopeTurnFlag = true;
			_states = &Rope::Shrinking;
		}
#ifdef _DEBUG
		DrawString(200, 300, "到着", 0xfffffff);
#endif
	}
	else {
	}
}

//ロープがプレイヤーのposに帰る処理
//最後まで変えればまた待機状態に移る
void Rope::Shrinking(Input* input)
{
	if (_state == ST_ROPE_SHRINKING)
	{
		if (--itr != ropeinfo.begin())
		{
			DrawRopeRect();

			if (_ropeDir != ROPE_DIR_NON)
			{
				SetRope();
			}
			else {
			}

			if (_curveDir != ROPE_DIR_CURVE_NON)
			{
				SetCurveRope();
			}
			else {
			}
		}
		else {
			itr = ropeinfo.begin();
			_vec = { 0,0 };
			RotationPos = { 0,-20 };
			_state = ST_ROPE_READY;
			_states = &Rope::Ready;
		}
#ifdef _DEBUG
		DrawString(200, 350, "帰宅中", 0xfffffff);
#endif
	}
	else {
	}
}

//パッドのスティックの角度によってロープの向きを決める
ROPE_DIR Rope::RopeDirSetPad(Input* input)
{
	if (_inputinfo.L_Stick.L_SensingFlag >= _minValue)
	{
		if (input->GetStickDir(_inputinfo.L_Stick.lstick) == SD_RIGHT && dirFlag ||
		input->GetStickDir(_inputinfo.L_Stick.lstick) == SD_LEFT && !dirFlag)
		{
			return _ropeDir = ROPE_DIR_STRAIGHT;
		}
		else if (input->GetStickDir(_inputinfo.L_Stick.lstick) == SD_UP)
		{
			return _ropeDir = ROPE_DIR_UPPER;
		}
		else if (input->GetStickDir(_inputinfo.L_Stick.lstick) == SD_DOWN)
		{
			return _ropeDir = ROPE_DIR_LOWER;
		}
		else {
		}
	}
	else {
	}
	return ROPE_DIR_NON;
}

//押したボタンによって角度を変えています(直線)
ROPE_DIR Rope::RopeDirSet(void)
{
	if (_key.keybit.L_UP_BUTTON && !_lastkey.keybit.L_UP_BUTTON)
	{
		return _ropeDir = ROPE_DIR_UPPER;
	}
	else if (_key.keybit.L_RIGHT_BUTTON && !_lastkey.keybit.L_RIGHT_BUTTON)
	{
		return _ropeDir = ROPE_DIR_STRAIGHT;
	}
	else if (_key.keybit.L_DOWN_BUTTON && !_lastkey.keybit.L_DOWN_BUTTON)
	{
		return _ropeDir = ROPE_DIR_LOWER;
	}
	else {
	}

	return ROPE_DIR_NON;
}

//押したボタンによって角度を変えています(曲線)
ROPE_CURVE_DIR Rope::CurveRopeDirSet(void)
{
	if (_key.keybit.START_BUTTON && !_lastkey.keybit.START_BUTTON)
	{
		return _curveDir = ROPE_DIR_CURVE_UP;
	}
	else if (_key.keybit.BACK_BUTTON && !_lastkey.keybit.BACK_BUTTON)
	{
		return _curveDir = ROPE_DIR_CURVE_LOW;
	}

	return _curveDir = ROPE_DIR_CURVE_NON;
}

//ROPE_DIRの状態によってradの値を決めます
//書き方としてはあんまりよくない
float Rope::SetRopeRad(void)
{
	switch (_ropeDir)
	{
	case ROPE_DIR_UPPER:
		return dirFlag ? AngleRad(-ROPE_THETA) : AngleRad(-ROPE_THETA2);
		break;

	case ROPE_DIR_STRAIGHT:
		return dirFlag ? AngleRad(0.f) : AngleRad(STRAIGHT_RAD);
		break;

	case ROPE_DIR_LOWER:
		return dirFlag ? AngleRad(ROPE_THETA) : AngleRad(ROPE_THETA2);
		break;

	case ROPE_DIR_NON:
		return AngleRad(0.f);
		break;

	default:
		break;
	}

	return AngleRad(0.f);

}

float Rope::SetRopeRadCurve(void)
{
	switch(_curveDir)
	{
	case ROPE_DIR_CURVE_UP:
		return theta = dirFlag ? -ROPE_THETA :-ROPE_THETA2;
		break;

	case ROPE_DIR_CURVE_LOW:
		return theta = dirFlag ? ROPE_THETA : ROPE_THETA2;
		break;

	case ROPE_DIR_CURVE_NON:
		break;

	default:
		return theta = 0.f;
		break;
	}

	return theta;

}

//スピードset用(ちょっと雑)
Position2 Rope::SetVec(void)
{
	if (RopeTurnFlag)
	{
		return {-ROPE_SPEED, -ROPE_SPEED};
	}
	else 
	{
		return { ROPE_SPEED, ROPE_SPEED};
	}

}

//ロープの先端のstateを返します
ROPE_STATE Rope::GetRopeState(void)
{
	//ST_ROPE_READY ロープを伸ばす準備のstate ここでプレイヤーは動いてok
	//ST_ROPE_EXTENDING ロープを伸ばしている間のstate
	//ST_ROPE_EXTENDED ロープが最大まで伸びきった時のstate
	//ST_ROPE_SHRINKING ロープを縮めている間のstate

	return _state;
}

//ロープの速度を返します
Position2 Rope::GetRopeVec(void)
{
	return _vec;
}

//ギミックなどと当たった時のロープのpos
Position2  Rope::GetReturnPos(void)
{
	return _HitPos;
}

////ロープの先端の矩形を返します(Rect)
Rect& Rope::GetRect(void)
{
	return _RopeRect;
}

//ロープの先端の矩形を返します(Circle)
Circle& Rope::GetCircle(void)
{
	return _RopeCircle;
}

//更新されたHitClassを受け取るための関数です
void Rope::GetClass(HitClass* h)
{
	_hit = h;
}