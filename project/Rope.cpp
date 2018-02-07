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
#include "ModelMgr.h"
#include "SoundMgr.h"
#include "ImageMgr.h"

Rope::Rope() 
{
	_tmpOffset.x = 0;
	_tmpOffset.y = 0;
}

Rope::Rope(Player* player)
{
	_player = player;
	_mapctl = MapCtl::GetInstance();
	_modelmgr = ModelMgr::Instance();
	RopeInit();
}

Rope::~Rope()
{
	_modelmgr->ModelIdAllDelete();
}

void Rope::Updata(Input* input,Position2 offset)
{
	SetRopeRadForDrawZ();
	DirChange();
	(this->*_states)(input);
	_tmpOffset = offset;
	_key = input->GetInput(1).key;
	_lastkey = input->GetLastKey();
	_inputinfo = input->GetInput(1);

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

	RopeTurnFlag = false;
	dirFlag      = false;
	padFlag      = false;

	tongueHitTurn = false;

	_vec = { 0.f,0.f };
	RopeAngle_Y = 0.f;
	RopeAngle_Z = 0.f;
	mentenanceNum_X = 0;
	mentenanceNum_Y = 0;
	ropeFiringTimer = 20;
	RopeHitModelNumY = 0;
	dirNum = 0;
	timerWait = WAIT_TIMER;
	ImageMgr& im = ImageMgr::Instance();

	//モデル読み込み
	modelhandle = _modelmgr->ModelIdReturn("Tongue_model/sita5.mv1", SCENE_RESULT);
	//それぞれのアニメーションをアタッチ+総時間の設定
	AnimAttachIndex = MV1AttachAnim(modelhandle,1,-1,false);
	AnimTotalTime = MV1GetAttachAnimTotalTime(modelhandle, AnimAttachIndex);
	//0～19 の20
	for (int j = 0; j < ROPE_LENGTH_MAX; j++)
	{
		ropeinfo.push_back(j);//先頭から入れる
	}
}
//ロープ描画処理
void Rope::DrawRopeRect(void)
{
	//circle
	_RopeCircle.SetCenter(_rope[*itr].x + _tmpOffset.x,SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY, range);
	_RopeCircle2.SetCenter(_rope[*itr].x + _tmpOffset.x, SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY - 10, range);

	ModelManager();

	_RopeCircle.Draw(_tmpOffset);
	_RopeCircle2.Draw(_tmpOffset);

}
void Rope::ModelManager(void)
{
	//当たったかどうかでアニメーションを進めるか戻す
	AnimNowTime = tongueHitTurn ? AnimNowTime -= 0.5f : AnimNowTime += 0.5f;
	//現在のアニメーションが最大フレームまでいったらループする
	if (AnimNowTime >= AnimTotalTime)
	{
		AnimNowTime = 0;
	}
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, RopeAngle_Y, RopeAngle_Z));
	//アニメーションをアタッチ
	MV1SetAttachAnimTime(modelhandle, AnimAttachIndex, AnimNowTime);
	//モデルのposを設定
	MV1SetPosition(modelhandle, VGet(_player->ReturnWoToScPos2ver().x - mentenanceNum_X, _player->ReturnWoToScPos2ver().y - mentenanceNum_Y, 0));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(0.3f, 0.3f, 0.3f));
	//モデルを描画 輪郭線0.0fで透過
	_modelmgr->Draw(modelhandle, 0.0f);
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

//プレイヤーの向きを見ていろいろ設定
void Rope::DirChange(void)
{
	if (_player->GetDir() == DIR_RIGHT)
	{
		dirFlag = true;
		RopeAngle_Y = AngleRad(90.f);
		dirNum = 45;
	}
	else if (_player->GetDir() == DIR_LEFT)
	{
		dirFlag = false;
		RopeAngle_Y = AngleRad(-90.f);
		dirNum = -15;
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
			_rope[*itr] = _player->ReturnWoToScPos2ver();
		}

		//発射準備のボタンが変わっています 右shiftキー or pad X
		if ((_key.keybit.L_LEFT_BUTTON && !_lastkey.keybit.L_LEFT_BUTTON || _key.keybit.L_UP_BUTTON && !_lastkey.keybit.L_UP_BUTTON )&& !padFlag ||
			_key.keybit.X_BUTTON && !_lastkey.keybit.X_BUTTON && padFlag)
		{
			RopeTurnFlag = false;
			_minValue = SV_MID;
			_HitPos = { 0,0 };
			_vec = { 0,0 };
			_state = ST_ROPE_SELECT;
			_states = &Rope::SelectDir;
		}
		else {
		}
#ifdef _DEBUG
		//DrawString(200, 200, "待機中", 0xfffffff);
#endif
	}
	else {
	}
}

//ロープの移動の方向を決める処理
//この状態のときは、the world状態
void Rope::SelectDir(Input* input)
{
	SoundMgr& so = SoundMgr::Instance();
	if (_state == ST_ROPE_SELECT)
	{
		ImageMgr& im = ImageMgr::Instance();

		//MV1SetRotationXYZ(modelhandle, VGet(0.f, RopeAngle_Y, RopeAngle_Z));
		//MV1SetPosition(modelhandle, VGet(_player->ReturnWoToScPos2ver().x - mentenanceNum_X, _player->ReturnWoToScPos2ver().y - mentenanceNum_Y,0.0f));
		//MV1SetScale(modelhandle, VGet(0.3f, 0.3f, 0.3f));
		//MV1DrawFrame(modelhandle,0);
		//_modelmgr->SetMaterialDotLine(modelhandle, 0.0f);

		dirdraw_Y = _player->GetStateCrouch() ? 50 : 0;
		dirdraw_X = _player->GetStateCrouch() ? (dirFlag ? 10 : -10) : 0;


		//矢印表示
		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			 _player->GetPos().y - _tmpOffset.y-5 + dirdraw_Y, 1.0, dirFlag ? AngleRad(45.0f) : AngleRad(135.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, false);

		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			_player->GetPos().y - _tmpOffset.y - 20+  dirdraw_Y, 1.0, AngleRad(0.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, !dirFlag);

		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			_player->GetPos().y - _tmpOffset.y - 35 + dirdraw_Y, 1.0, dirFlag ? AngleRad(-45.0f) : AngleRad(-135.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, false);



		//ロープ待機解除 Readyの状態に戻す
		if (_key.keybit.L_LEFT_BUTTON && !_lastkey.keybit.L_LEFT_BUTTON && !padFlag ||
			_key.keybit.X_BUTTON && !_lastkey.keybit.X_BUTTON && padFlag)
		{
			_vec = { 0,0 };
			_state = ST_ROPE_READY;
			_states = &Rope::Ready;
		}

		//押したボタン or 入れた角度によって向きを決める
		//ここだけのために関数ポインタの引数にinputを設定している(怒)
		if ((RopeDirSet() || RopeDirSetPad(input)) != ROPE_DIR_NON ||
			CurveRopeDirSet() != ROPE_DIR_CURVE_NON)
		{
			RopeTurnFlag = false;
			_vec = { 0,0 };
			itr = ropeinfo.begin();
			_state = ST_ROPE_EXTENDING;
			_states = &Rope::Extending;
			so.SeStart("Se/extend4.mp3",SCENE_RESULT);
		}
	}
#ifdef _DEBUG
	//DrawString(200, 200, "選択中", 0xfffffff);
#endif
}

//ロープの行きの処理
//ROPE_LENGTH_MAXまで伸び続ける
//最大まで行くと待ちの処理に移る
void Rope::Extending(Input* input)
{
	if (_state == ST_ROPE_EXTENDING)
	{
		if (--ropeFiringTimer < 0)
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

				//伸ばしている最中にギミックやステージにあたれば強制的に戻す
				if (_hit->GimmickHit(GetCircle()) || _hit->EnemyHit(GetCircle()) || _hit->EnemyHit(GetCircle2())||
					_mapctl->GetChipType(Position2(_rope[*itr].x + _tmpOffset.x, SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY)) == CHIP_N_CLIMB_WALL ||
					_mapctl->GetChipType(Position2(_rope[*itr].x + _tmpOffset.x, SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY)) == CHIP_CLIMB_WALL ||
					_mapctl->GetChipType(Position2(_rope[*itr].x + _tmpOffset.x, SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY - 10)) == CHIP_N_CLIMB_WALL ||
					_mapctl->GetChipType(Position2(_rope[*itr].x + _tmpOffset.x, SCREEN_SIZE_Y - _rope[*itr].y + _tmpOffset.y - RopeHitModelNumY - 10)) == CHIP_CLIMB_WALL)
				{
					_HitPos = _rope[*itr];
					timerWait = 0;
					itr = last;
					_state = ST_ROPE_SHRINKING;
					tongueHitTurn = true;
					RopeTurnFlag = true;
					_states = &Rope::Shrinking;
				}
				else {
				}
			}
			else
			{
				_state = ST_ROPE_EXTENDED;
				_states = &Rope::Extended;
			}
		}
#ifdef _DEBUG
		//DrawString(200, 250, "移動中", 0xfffffff);
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
		timerWait--;
		//当たったかどうかでアニメーションを進めるか戻す
		ModelManager();

		if (timerWait < 0)
		{
			itr = last;
			_state = ST_ROPE_SHRINKING;
			RopeTurnFlag = true;
			_states = &Rope::Shrinking;
		}

#ifdef _DEBUG
		//DrawString(200, 300, "到着", 0xfffffff);
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
		}
		else {
			itr = ropeinfo.begin();
			_vec = { 0,0 };
			AnimNowTime = 0;
			timerWait = WAIT_TIMER;
			tongueHitTurn = false;
			ropeFiringTimer = 20;
			_state = ST_ROPE_READY;
			_states = &Rope::Ready;
		}
#ifdef _DEBUG
		//DrawString(200, 350, "帰宅中", 0xfffffff);
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
	if (_key.keybit.R_UP_BUTTON && !_lastkey.keybit.R_UP_BUTTON)
	{
		return _ropeDir = ROPE_DIR_UPPER;
	}
	else if ((dirFlag && _key.keybit.R_RIGHT_BUTTON && !_lastkey.keybit.R_RIGHT_BUTTON) ||
		(!dirFlag && _key.keybit.R_LEFT_BUTTON && !_lastkey.keybit.R_LEFT_BUTTON))
	{
		return _ropeDir = ROPE_DIR_STRAIGHT;
	}
	else if (_key.keybit.R_DOWN_BUTTON && !_lastkey.keybit.R_DOWN_BUTTON)
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

//ROPE_DIRの状態によってradの値を決めます()
float Rope::SetRopeRad(void)
{
	switch (_ropeDir)
	{
	case ROPE_DIR_UPPER:
		return dirFlag ? AngleRad(ROPE_THETA) : AngleRad(ROPE_THETA2);
		break;

	case ROPE_DIR_STRAIGHT:
		return  dirFlag ? AngleRad(0.0f) : AngleRad(-180.0f);
		break;

	case ROPE_DIR_LOWER:
		return dirFlag ? AngleRad(-ROPE_THETA) : AngleRad(-ROPE_THETA2);
		break;

	case ROPE_DIR_NON:
		return AngleRad(0.f);
		break;

	default:
		break;
	}

	return AngleRad(0.f);
}

//float Rope::SetRopeRadCurve(void)
//{
//	switch(_curveDir)
//	{
//	case ROPE_DIR_CURVE_UP:
//		return theta = dirFlag ? -ROPE_THETA :-ROPE_THETA2;
//		break;
//
//	case ROPE_DIR_CURVE_LOW:
//		return theta = dirFlag ? ROPE_THETA : ROPE_THETA2;
//		break;
//
//	case ROPE_DIR_CURVE_NON:
//		break;
//
//	default:
//		return theta = 0.f;
//		break;
//	}
//
//	return theta;
//
//}

//3D用舌の角度とかを設定してます
void Rope::SetRopeRadForDrawZ(void)
{
	switch (_ropeDir)
	{
	case ROPE_DIR_UPPER:
		RopeAngle_Z = dirFlag ? AngleRad(ROPE_THETA) : AngleRad(-ROPE_THETA);
		RopeHitModelNumY = _player->GetStateCrouch() ? 10 :dirFlag ? 35:25;//あたり判定用
		mentenanceNum_Y = _player->GetStateCrouch() ? -28 + _RopeRect.h / 2 :-58 + _RopeRect.h / 2;//モデル描画
		mentenanceNum_X = dirFlag ? -20 : 20;
		break;
	case ROPE_DIR_LOWER:
		RopeAngle_Z =  dirFlag ? AngleRad(-ROPE_THETA) : AngleRad(ROPE_THETA);
		RopeHitModelNumY = _player->GetStateCrouch() ? 45:70;//あたり判定用
		mentenanceNum_Y = _player->GetStateCrouch() ? -28 + _RopeRect.h / 2 : -58 + _RopeRect.h / 2;//モデル描画
		mentenanceNum_X = dirFlag ? -20 : 20;
		break;
	case ROPE_DIR_NON:
	case ROPE_DIR_STRAIGHT:
		RopeAngle_Z = AngleRad(0.f);
		RopeHitModelNumY = _player->GetStateCrouch() ?20:55;//あたり判定用
		mentenanceNum_Y = _player->GetStateCrouch() ? -30 + _RopeRect.h / 2: -60 +_RopeRect.h / 2;//モデル描画
		mentenanceNum_X = dirFlag ? -10 : 10;
		break;
	default:
		break;
	}
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

Circle& Rope::GetCircle2(void)
{
	return _RopeCircle2;
}

//更新されたHitClassを受け取るための関数です
void Rope::GetClass(HitClass* h)
{
	_hit = h;
}