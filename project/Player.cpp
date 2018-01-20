#include "DxLib.h"
#include "GameScene.h"
#include "GimDrop.h"
#include "GimPull.h"
#include "HitClass.h"
#include "MapCtl.h"
#include "Player.h"
#include "Geometry.h"
#include "Rope.h"

#include <iostream>
using namespace std;


Player::Player()
{
	_pos.x = 200.0f;
	_pos.y = 300.0f;
	initPos.x = _pos.x;
	initPos.y = _pos.y;
	_state = ST_DEF;
	vx = 0.0f;
	vy = 0.0f;
	vanCnt = 60 * VANISH_CNT;	//とりあえず３秒
	feverFlag = false;
	feverTime = 60 * FEVER_CNT;
	//_hit = new HitClass();
	_plRect.w = 32;
	_plRect.h = 32;
	_plRect.SetCenter(_pos.x + (_plRect.w / 2) , _pos.y + (_plRect.h / 2) );
	_map = MapCtl::GetInstance();
	//_rope = new Rope(this);
	WallFlag = false;
	moveFlag = false;
	_minSensingValueL = SV_HIGH;

}
Player::~Player()
{
	//delete _hit;
	//delete _rope;
}
//更新されたHitClassを受け取るための関数です
void Player::Getclass(HitClass* h, Rope*r)
{
	_hit = h;
	_rope = r;
}
void Player::Update(Input* input)
{
	_key = input->GetInput(1).key;
	_lastKey = input->GetLastKey();
	_inpInfo = input->GetInput(1);
	memcpy(oldkeyData, keyData, sizeof(keyData));
	GetHitKeyStateAll(keyData);

	if (feverFlag == true) {
		FeverGravity();
		FeverUpdata(input);
	}
	else if (feverFlag == false) {
		//重力
		gravity();
		//移動制御
		setMove(input);
	}
	//ｽﾃｰﾀｽ制御
	setState();
	//HitToEnemy();		//敵と当たったとき
}

//移動系の処理
void Player::setMove(Input* input)
{
	setDir(input);
	moveJump();
	//setState();
	moveWall();
	moveRope();
	accelePL();
	EnterDoor();
}

//ｽﾃｰﾀｽ系の処理
void Player::setState(void)
{
	stFever();
	if (_state != ST_FEVER) {
		stVanish();
	}
}
void Player::FeverUpdata(Input* input)
{
	setDir(input);
	FeverJump();
	moveWall();
	moveRope();
	moveFever();
	EnterDoor();
}
//向きを決める
void Player::setDir(Input* input)
{
	//ﾛｰﾌﾟ状態なら向きは変えられない
	float angle = static_cast<int>((RadAngle(_inpInfo.L_Stick.lstick) * 100.f));

	if (_state != ST_ROPE) {
		//右
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON  ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_RIGHT;
			_state = ST_MOVE;
		}
		//左
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_LEFT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_LEFT;
			_state = ST_MOVE;
		}
		//上
		else if (_inpInfo.key.keybit.R_UP_BUTTON ||
			input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_UP &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_UP;
		}
		else if (_inpInfo.key.keybit.R_DOWN_BUTTON ||
			input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_DOWN &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_DOWN;
		}
		else if(!_inpInfo.key.keybit.R_LEFT_BUTTON || 	!_inpInfo.key.keybit.R_RIGHT_BUTTON ||
			!_inpInfo.key.keybit.R_UP_BUTTON || !_inpInfo.key.keybit.R_DOWN_BUTTON ||
			input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_RIGHT||input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_LEFT||
			input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_UP   ||input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_DOWN){
			//押してない
			_dir = DIR_NON;
			_state = ST_STOP;
		}
		else {
		}
	}
	else {
		_state = ST_STOP;
	}
#ifdef _DEBUG
	DrawFormatString(80, 260, 0xffffff, "%f", angle);
#endif
}
//移動制御
bool Player::accelePL(void)
{
	InputSetMove();
	//マップとの当たり判定
	//2ドットほど判定を狭めている
	//右
	Position2 nextPosRight[2];
	//右下	
	nextPosRight[0].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[0].y = _pos.y + (_plRect.h - 1);
	//右上
	nextPosRight[1].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//登れる壁と登れない壁、ギミックとの判定
		if (_map->GetChipType(nextPosRight[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosRight[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosRight[j]) && (_hit->GimmickHitType(nextPosRight[j]) != GIM_FALL)&&_hit->GimmickHitType(nextPosRight[j])!= GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	
	//
	//左
	Position2 nextPosLeft[2];
	//左下
	nextPosLeft[0].x = _pos.x + vx + 2;
	nextPosLeft[0].y = _pos.y + (_plRect.h - 1);
	//左上
	nextPosLeft[1].x = _pos.x + vx + 2;
	nextPosLeft[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//登れる壁と登れない壁、ギミックとの判定
		if (_map->GetChipType(nextPosLeft[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosLeft[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosLeft[j]) && _hit->GimmickHitType(nextPosLeft[j]) != GIM_FALL&&_hit->GimmickHitType(nextPosLeft[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	//加速度を足す
	_pos.x += (int)vx;
	return false;
}
void Player::InputSetMove()
{
	//キーボードとパッドで移動処理を分けています
	//移動(ゲームパッド)
	if (_inpInfo.num >= 1)
	{
		//ここのL_SensingFlagがうまく更新されていないとみた!! (error中)
		//入力しっぱなしでコンパイルすると動きます
		if (_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)
		{
			if ((vx - ACCEL_X)< playerSpeedTable[_inpInfo.L_Stick.L_SensingFlag])
			{
				if (_dir == DIR_RIGHT)
				{
					vx += ACCEL_X;
					if (vx > MAX_SPEED) {
						vx = MAX_SPEED;
					}
				}
				else if (_dir == DIR_LEFT)
				{
					vx -= ACCEL_X;
					if (vx < -MAX_SPEED) {
						vx = -MAX_SPEED;
					}
				}
				else {
				}
			}
			else {
				vx = playerSpeedTable[_inpInfo.L_Stick.L_SensingFlag];
			}
		}
		else {
			if (JumpFlag == false) {
				if (vx < 0) {
					vx++;
				}
				if (vx > 0) {
					vx--;
				}
			}
		}
	}
	else
	{
		//移動(キーボード)
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON)
		{
			vx += ACCEL_X;
			if (vx > MAX_SPEED) {
				vx = MAX_SPEED;
			}
		}
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
			vx -= ACCEL_X;
			if (vx < -MAX_SPEED) {
				vx = -MAX_SPEED;
			}
		}
		else {
			if (JumpFlag == false) {
				if (vx < 0) {
					vx++;
				}
				if (vx > 0) {
					vx--;
				}
			}
		}
	}
}
//壁移動の処理
bool Player::moveWall(void)
{
	if (WallFlag == true) {
		_state = ST_WALL;
	}
	int count =0;
	//壁登り状態
	//操作性に難あり
	Position2 nextPos[4];
	//右下
	nextPos[0].x = _pos.x  + _plRect.w;
	nextPos[0].y = _pos.y + (_plRect.h - 1);
	//左下
	nextPos[1].x = _pos.x ;
	nextPos[1].y = _pos.y + (_plRect.h - 1);
	//右上
	nextPos[2].x = _pos.x + _plRect.w;
	nextPos[2].y = _pos.y;
	//左上
	nextPos[3].x = _pos.x;
	nextPos[3].y = _pos.y;
	//ﾌﾟﾚｲﾔｰの下、マップチップ1分下
	Position2 downPos;
	downPos.x = _pos.x + (_plRect.w / 2);
	downPos.y = _pos.y + _plRect.h+ MAP_CHIP_SIZE_Y;
	//壁登り状態にする条件
	for (int j = 0; j < 4; j++) {
		if (_map->GetChipType(nextPos[j]) == CHIP_CLIMB_WALL) {
			count = 0;
			//壁が近くにあったとき、ボタンを押すと壁に張り付く
			if (_inpInfo.num >= 1)
			{
				if (WallFlag == false) {
					if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
						WallFlag = true;
						break;
					}
				}
				else {
					if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
						WallFlag = false;
						break;
					}
				}
			}
			else {
				if (WallFlag == false) {
					if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
						WallFlag = true;
						break;
					}
				}
				else {
					if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
						WallFlag = false;
						break;
					}
				}
			}
			//もし足元に床がなければそのまま壁に張り付く
			if (_map->GetChipType(downPos) == CHIP_BLANK) {
				WallFlag = true;
				break;
			}
		}
		else {
			count++;
		}
	}
	if (count >= 4) {
		WallFlag = false;
	}

	moveFlag = false;
	//壁の移動制限
	Position2 WallPosMiddl[2],WallPosTop[2],WallPosBottom[2];
	//右（真ん中）
	WallPosMiddl[0].x = _pos.x + _plRect.w;
	WallPosMiddl[0].y = _pos.y + (_plRect.h / 2);
	//左（真ん中）
	WallPosMiddl[1].x = _pos.x;
	WallPosMiddl[1].y = _pos.y + (_plRect.h / 2);
	//右上
	WallPosTop[0].x = _pos.x + _plRect.w;
	WallPosTop[0].y = _pos.y;
	//左上
	WallPosTop[1].x = _pos.x;
	WallPosTop[1].y = _pos.y;
	//補正のために下も確認する
	//右下
	WallPosBottom[0].x = _pos.x + _plRect.w;
	WallPosBottom[0].y = _pos.y + (_plRect.h-1);
	//左下
	WallPosBottom[1].x = _pos.x;
	WallPosBottom[1].y = _pos.y + (_plRect.h - 1);
	for (int j = 0; j < 2; j++) {
		//ｷｬﾗの半分以上,上はいけないようにする
		if (_rope->GetRopeState() != ST_ROPE_READY) {
			moveFlag = false;
			break;
		}
		if (_map->GetChipType(WallPosMiddl[j]) == CHIP_CLIMB_WALL
		   || _map->GetChipType(WallPosTop[j]) != CHIP_BLANK) {
			moveFlag = true;
			break;
		}
		else {
			moveFlag = false;
		}
	}
	//半分以上で壁に張り付いてしまったときは半分まで下げる
	Position2 offsetPos[2];
	offsetPos[0].x = WallPosMiddl[0].x;
	offsetPos[0].y = WallPosMiddl[0].y + 3.0f;
	offsetPos[1].x = WallPosMiddl[1].x -1.0f;
	offsetPos[1].y = WallPosMiddl[1].y + 3.0f;
	if (_state == ST_WALL) {
		if (moveFlag == false) {

			for (int f = 0; f < 2; f++) {
				if(_map->GetChipType(offsetPos[f]) != CHIP_CLIMB_WALL
					&& _map->GetChipType(WallPosBottom[f]) == CHIP_CLIMB_WALL){
					_pos.y++;
				}
				else {		
					continue;
				}
			}
		}
	}

	if (_state == ST_WALL) {
		//壁の中で移動可能なら
		if (moveFlag) {
			if (_inpInfo.num >= 1)
			{
				if (_dir == DIR_UP || _key.keybit.L_UP_BUTTON)
				{
					vy = -WALL_SPEED;
				}
				else if (_dir == DIR_DOWN  ||_key.keybit.L_DOWN_BUTTON)
				{
					vy = WALL_SPEED;
				}
				else {
					vy = 0.0f;
				}
			}
			else {
				if (_inpInfo.key.keybit.R_UP_BUTTON) {
					vy = -WALL_SPEED;
				}
				else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {
					vy = WALL_SPEED;
				}
				else {
					vy = 0.0f;
				}
			}
		}
		else if (_rope->GetRopeState() != ST_ROPE_READY) {

		}
		else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {		//キーボード
			vy = WALL_SPEED;
		}
		else if (_dir == DIR_DOWN){
			vy = WALL_SPEED;
		}
		else {
			vy = 0.0f;
		}


		//下が地面だった時は止まる
		Position2 nextPosDown;
		nextPosDown.x = _pos.x + (_plRect.w / 2);
		nextPosDown.y = _pos.y + vy + (_plRect.h - 1);
		if (_map->GetChipType(nextPosDown) == CHIP_CLIMB_WALL
		 || _map->GetChipType(nextPosDown) == CHIP_N_CLIMB_WALL) {
			vy = 0.0f;
		}
		//上が壁だったときは止まる
		Position2 nextPosUp;
		nextPosUp.x = _pos.x + (_plRect.w/2);
		nextPosUp.y = _pos.y + vy;
		if (_map->GetChipType(nextPosUp) == CHIP_CLIMB_WALL
			|| _map->GetChipType(nextPosUp) == CHIP_N_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUp) == GIM_ATTRACT) {
			vy = 0.0f;
		}

		//位置補正
		Position2 tmpPos,WallPosDownL,WallPosDownR;
		//右下
		WallPosDownR.x = _pos.x + _plRect.w;
		WallPosDownR.y = _pos.y + (_plRect.h - 1);
		//左下
		WallPosDownL.x = _pos.x;
		WallPosDownL.y = _pos.y + (_plRect.h - 1);

		tmpPos.y = (_pos.y - _plRect.h / 2) / 32 * 32;
		//moveFlagがfalseのときは位置補正を行う
		if (!moveFlag) {
			if (_rope->GetRopeState() != ST_ROPE_READY) {

			}
			else if (_inpInfo.num >= 1)
			{		//パッドの場合
				if (_dir == DIR_RIGHT) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
				if (_dir == DIR_LEFT) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
			}
			else {	//キーボードの場合
				if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !_lastKey.keybit.R_RIGHT_BUTTON) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
				if (_inpInfo.key.keybit.R_LEFT_BUTTON && !_lastKey.keybit.R_LEFT_BUTTON) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
			}
		}
		_pos.y += vy;
	}

#ifdef _DEBUG
	DrawFormatString(10, 430, 0xffffff, "flag::%d", moveFlag);
#endif // _DEBUG
	return false;
}

//ﾛｰﾌﾟ状態の処理
bool Player::moveRope(void)
{

	//ﾛｰﾌﾟ状態なら動けない
	if (_rope->GetRopeState() != ST_ROPE_READY) {
		_state = ST_ROPE;

		if (vx != 0 && JumpFlag) {
			moveRopeJumpFlag = true;
		}
		if (moveFlag == true) {
			moveFlag = false;
		}
		vx = 0.0f;
	}

	//勢いを殺さずに着地する
	if (_state == ST_JUMP) {
		if (_dir == DIR_RIGHT) {
			if (moveRopeJumpFlag) {
				vx = MAX_SPEED;
			}
		}
		if (_dir == DIR_LEFT) {
			if (moveRopeJumpFlag) {
				vx = -MAX_SPEED;
			}
		}
	}
	return false;
}
void Player::moveFever()
{
	InputSetMove();
	Position2 nextPosRight[2];
	//右下	
	nextPosRight[0].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[0].y = _pos.y + (_plRect.h - 1);
	//右上
	nextPosRight[1].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//登れる壁と登れない壁、ギミックとの判定
		if (_map->GetChipType(nextPosRight[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosRight[j]) && (_hit->GimmickHitType(nextPosRight[j]) != GIM_FALL) && _hit->GimmickHitType(nextPosRight[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}

	//
	//左
	Position2 nextPosLeft[2];
	//左下
	nextPosLeft[0].x = _pos.x + vx + 2;
	nextPosLeft[0].y = _pos.y + (_plRect.h - 1);
	//左上
	nextPosLeft[1].x = _pos.x + vx + 2;
	nextPosLeft[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//登れる壁と登れない壁、ギミックとの判定
		if (_map->GetChipType(nextPosLeft[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosLeft[j]) && _hit->GimmickHitType(nextPosLeft[j]) != GIM_FALL&&_hit->GimmickHitType(nextPosLeft[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	//加速度を足す
	_pos.x += (int)vx;

}
//ｽﾃﾙｽ状態の処理
//3秒動かなかったら消える
bool Player::stVanish(void)
{
	//3秒後消える
	if (vanCnt > 0) {
		vanCnt--;
	}
	//動いていたらｶｳﾝﾄを戻す
	//壁登り状態で動いていたらｽﾃﾙｽにならない
	if (_state == ST_MOVE||_state==ST_JUMP||_state==ST_ROPE||vy!=0) {
		vanCnt = 60 * VANISH_CNT;
	}

	if (vanCnt <= 0) {
		_state = ST_VANISH;
	}
#ifdef _DEBUG
	DrawFormatString(0, 120, 0xffffff, "%d", vanCnt);
#endif
	return false;
}

//ﾌｨｰﾊﾞｰ処理
bool Player::stFever(void)
{
	//とりあえずﾌｨｰﾊﾞｰ
	if (keyData[KEY_INPUT_Z]) {
		if (feverFlag == false) {
			feverFlag = true;
		}
	}
	if (feverFlag == true) {
		_state = ST_FEVER;
		feverTime--;
	}
	if (feverTime < 0) {
		feverFlag = false;
		feverTime = 60 * FEVER_CNT;
	}
#ifdef _DEBUG
	DrawFormatString(600,10,0xffffff,"%d",feverTime);
#endif
	return false;
}

//ｼﾞｬﾝﾌﾟ処理
bool Player::moveJump(void)
{
	//flagがtrueならｼﾞｬﾝﾌﾟ状態
	if (JumpFlag == true) {
		_state = ST_JUMP;
	}
	//ｼﾞｬﾝﾌﾟ
	if (JumpFlag == false) {
		if (_inpInfo.num >= 1) {
			if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
				vy = -JUMP_POWER;
				JumpFlag = true;
			}
		}
		else {
			if (keyData[KEY_INPUT_SPACE] ^ oldkeyData[KEY_INPUT_SPACE] & keyData[KEY_INPUT_SPACE]) {
				vy = -JUMP_POWER;
				JumpFlag = true;
			}
		}

	}
	else {
		//放物線を見せるために加速度にMAX_SPEEDを設定
		//良くない
		if (_state == ST_ROPE) {
			if (_inpInfo.key.keybit.R_RIGHT_BUTTON) {
				vx = MAX_SPEED;
			}
			if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
				vx = -MAX_SPEED;
			}
		}
	}

	//マップとの判定
	//2ドットほど判定を狭めている
	//上部の当たり判定
	Position2 nextPosUP[2];
	//右上	
	nextPosUP[0].x = _pos.x + (_plRect.w - 2);
	nextPosUP[0].y = _pos.y + (vy / 2);
	//左上
	nextPosUP[1].x = _pos.x + 2;
	nextPosUP[1].y = _pos.y + (vy / 2);
		//登れる壁、登れない壁との判定
	for (int j = 0; j < 2; j++) {
		if (_map->GetChipType(nextPosUP[j]) == CHIP_N_CLIMB_WALL
			|| _map->GetChipType(nextPosUP[j]) == CHIP_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUP[j]) == GIM_ATTRACT) {
			vy = 0.0f;
			break;
		}
	}
	return false;
}
void Player::FeverJump()
{
	//flagがtrueならｼﾞｬﾝﾌﾟ状態
	if (JumpFlag == true) {
		_state = ST_JUMP;
	}
	//ｼﾞｬﾝﾌﾟ
	if (JumpFlag == false) {
		if (_inpInfo.num >= 1) {
			if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
				vy = -JUMP_POWER;
				JumpFlag = true;
			}
		}
		else {
			if (keyData[KEY_INPUT_SPACE] ^ oldkeyData[KEY_INPUT_SPACE] & keyData[KEY_INPUT_SPACE]) {
				vy = -JUMP_POWER;
				JumpFlag = true;
			}
		}
	}
	else {
		//放物線を見せるために加速度にMAX_SPEEDを設定
		//良くない
		if (_state == ST_ROPE) {
			if (_inpInfo.key.keybit.R_RIGHT_BUTTON) {
				vx = MAX_SPEED;
			}
			if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
				vx = -MAX_SPEED;
			}
		}
	}
	//マップとの判定
	//2ドットほど判定を狭めている
	//上部の当たり判定
	Position2 nextPosUP[2];
	//右上	
	nextPosUP[0].x = _pos.x + (_plRect.w - 2);
	nextPosUP[0].y = _pos.y + (vy / 2);
	//左上
	nextPosUP[1].x = _pos.x + 2;
	nextPosUP[1].y = _pos.y + (vy / 2);
	for (int j = 0; j < 2; j++) {
		if (_map->GetChipType(nextPosUP[j]) == CHIP_N_CLIMB_WALL
			||_hit->GimmickHitType(nextPosUP[j]) == GIM_ATTRACT) {
			vy = 0.0f;
			break;
		}
	}
}
void Player::Draw(Position2& offset)
{
	//時機
	DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0xffffff, true);
	switch (_state)
	{
		//ｽﾃﾙｽ状態
	case ST_VANISH:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x  + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0xff0000, true);
		break;
		//ﾛｰﾌﾟ状態
	case ST_ROPE:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x  + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0x00ffff, true);
		break;
		//壁登り状態
	case ST_WALL:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y  + 32 -offset.y, 0xff00ff, true);
		break;
		//ﾌｨｰﾊﾞｰ状態
	case ST_FEVER:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0x0000ff, true);
		DrawString((int)_pos.x  - 20 -offset.x, (int)_pos.y  - 20 -offset.y, "＼FEVER／", 0x0000ff);
		break;
	default:
		break;
	}
	_plRect.SetCenter(_pos.x + (_plRect.w / 2), _pos.y + (_plRect.h / 2));

#ifdef _DEBUG
	DrawString(400, 200, "赤：ステルス状態", 0xffffff);
	DrawString(400, 220, "水：ﾛｰﾌﾟ使用状態", 0xffffff);
	DrawString(400, 180, "Lｺﾝﾄﾛｰﾙでﾛｰﾌﾟ使用（仮）", 0xffffff);
	DrawFormatString(10, 400, 0xffffff, "ｽﾃｰﾀｽ：%d", GetcharState());
	DrawFormatString(10, 415, 0xffffff, "dir:%d 左:2 右:3", _dir);
	_plRect.Draw(offset);
#endif
}

//敵と当たった時の処理を行う
void Player::HitToEnemy()
{
	if (_hit->EnemyHit(*this)) {
		_state = ST_DETH;
	}
	else
	{
	}
}
//重力
void Player::gravity(void)
{
	//壁登り状態なら重力は無視
	if (_state == ST_WALL) {
		return;
	}
	//マップとの判定
	//2ドットほど判定を狭めている
	Position2 nextPosDown[3];
	//右下	
	nextPosDown[0].x = _pos.x + (_plRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_plRect.h);
	//左下
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_plRect.h);
	//真ん中
	nextPosDown[2].x = _pos.x + (_plRect.w / 2);
	nextPosDown[2].y = _pos.y + (vy / 2) + (_plRect.h);
	//登れる壁、登れない壁との判定
	for (int j = 0; j < 3; j++) {
		if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosDown[j]) && _hit->GimmickHitType(nextPosDown[j]) != GIM_FALL && _hit->GimmickHitType(nextPosDown[j]) != GIM_DOOR)) {
			vy = 0.0f;
			JumpFlag = false;
			break;
		}
		else {
			//下部に何もなかったら重力を足す
			//速度調整のため3で割っている
			vy += GRAVITY / 3.0f;
			//一応Max値を設定しておく
			if (vy > MAX_GRAVITY) {
				vy = MAX_GRAVITY;
			}
			//空中だったらとりあえずｼﾞｬﾝﾌﾟ状態
			JumpFlag = true;
		}
	}
	//ﾛｰﾌﾟ状態ならうごけない
	if (_state == ST_ROPE) {
		vy = 0.0f;
	}
	//加速度を足す
	//速度調整のため２で割っている
	_pos.y += (int)vy / 2.0f;
}
//フィーバー時の重力です
void Player::FeverGravity()
{
	//壁登り状態なら重力は無視
	if (_state == ST_WALL) {
		return;
	}
	//マップとの判定
	//2ドットほど判定を狭めている
	Position2 nextPosDown[3];
	//右下	
	nextPosDown[0].x = _pos.x + (_plRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_plRect.h);
	//左下
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_plRect.h);
	//真ん中
	nextPosDown[2].x = _pos.x + (_plRect.w / 2);
	nextPosDown[2].y = _pos.y + (vy / 2) + (_plRect.h);
	//苦肉の策
	Position2 nextPosDown2[3];
	//右下
	nextPosDown2[0].x = nextPosDown[0].x;
	nextPosDown2[0].y = nextPosDown[0].y - 2;
	//左下
	nextPosDown2[1].x = nextPosDown[1].x;
	nextPosDown2[1].y = nextPosDown[1].y - 2;
	//真ん中
	nextPosDown2[2].x = nextPosDown[2].x;
	nextPosDown2[2].y = nextPosDown[2].y - 2;
	//登れる壁、登れない壁との判定
	for (int j = 0; j < 3; j++) {
/*		if (_map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL && (_map->GetMapNum(nextPosDown[j]) != _map->GetMapNum(nextPosDown2[j]))) {
			vy = 0.0f;
			JumpFlag = false;
			break;
		}
		else*/ if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosDown[j]) && _hit->GimmickHitType(nextPosDown[j]) != GIM_FALL && _hit->GimmickHitType(nextPosDown[j]) != GIM_DOOR)) {
			vy = 0.0f;
			JumpFlag = false;
			break;
		}
		else {
			//下部に何もなかったら重力を足す
			//速度調整のため3で割っている
			vy += GRAVITY / 3.0f;
			//一応Max値を設定しておく
			if (vy > MAX_GRAVITY) {
				vy = MAX_GRAVITY;
			}
			//空中だったらとりあえずｼﾞｬﾝﾌﾟ状態
			JumpFlag = true;
		}
	}if (JumpFlag == true && vy > 0) {
		for (int j = 0; j < 3; j++) {
			if (_map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL && (_map->GetMapNum(nextPosDown[j]) != _map->GetMapNum(nextPosDown2[j]))) {
				vy = 0.0f;
				JumpFlag = false;
				break;
			}
		}
	}
	//ﾛｰﾌﾟ状態ならうごけない
	if (_state == ST_ROPE) {
		vy = 0.0f;
	}
	//加速度を足す
	//速度調整のため２で割っている
	_pos.y += (int)vy / 2.0f;
}
//Rect取得
Rect& Player::GetRect()
{
	return _plRect;
}

//ｽﾃｰﾀｽ取得
CHAR_ST Player::GetcharState(void)
{
	return _state;
}

//_pos取得
Position2& Player::GetPos(void)
{
	return _pos;
}
//向き取得
DIR Player::GetDir(void)
{
	return _dir;
}
//初期位置に戻す
void Player::SetInitPos()
{
	_pos = initPos;
	//加速度も元に戻す
	vx = 0.0f;
	vy = 0.0f;
	_state = ST_DEF;
}
//初期位置をセットする
void Player::SetInitPos(Position2 p)
{
	_pos = p;
	initPos = _pos;
}
bool Player::EnterDoor()
{
	if (_hit->GimmickEnter(*this)) {
		if (_inpInfo.key.keybit.R_UP_BUTTON) {
			DrawString(30, 360, "クリアしたよ", GetColor(255, 255, 255));
			return true;
		}
	}
	return false;
}
void Player::SetRetryPos(Position2 midPos)
{
	_pos = midPos;
	//加速度も元に戻す
	vx = 0.0f;
	vy = 0.0f;
	_state = ST_DEF;
}