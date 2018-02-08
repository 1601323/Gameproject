#include <iostream>
#include <cmath>
#include "DxLib.h"
#include "GameScene.h"
#include "GimDrop.h"
#include "GimPull.h"
#include "HitClass.h"
#include "MapCtl.h"
#include "Player.h"
#include "Geometry.h"
#include "Rope.h"
#include "SoundMgr.h"
#include "ModelMgr.h"
#include "ImageMgr.h"

using namespace std;

Player::Player()
{
	_pos.x = 200.0f;
	_pos.y = 300.0f;
	initPos.x = _pos.x;
	initPos.y = _pos.y;
	initPos.z = _pos.z;
	_tmpPos.x = _pos.x;
	_tmpPos.y = _pos.y;
	_state = ST_DEF;
	vx = 0.0f;
	vy = 0.0f;
	vanCnt = 60 * VANISH_CNT;	//とりあえず３秒
	feverFlag = false;
	feverTime = 60 * FEVER_CNT;
	inviCnt = INVINCIBLETIMER * 60;
	//_hit = new HitClass();
	_plRect.w = 32;
	_plRect.h = 50;
	_wallRect.w = 32;
	_wallRect.h = 32;
	_plRect.SetCenter(_pos.x + (_plRect.w / 2), _pos.y  -(_plRect.h / 2));
	_wallRect.SetCenter(_pos.x+(_wallRect.w/2),_pos.y+(_wallRect.h/2));
	_map = MapCtl::GetInstance();
	//_rope = new Rope(this);
	WallFlag = false;
	moveFlag = false;
	fMoveRight = true;
	fMoveLeft = true;
	deathFlag = true;
	helpFever = false;
	airFlag = false;
	crouchFlag = false;
	vanFlag = false;
	ropeFlag = false;
	inviFlag = false;
	tmpFlag = false;
	WallAnimationFlag = false;
	_minSensingValueL = SV_HIGH;
	alfa = 255;
	tranceMax = 50;
	modelDirAngle = 0.0f;
	LineNum = 0.f;
	airCnt = 0;
	_fd = FEVER_DATA();

	_modelmgr = ModelMgr::Instance();
	//モデル読み込み
	modelhandle = MV1LoadModel("player_model/player.pmx");
	//それぞれのアニメーションをアタッチ+総時間の設定
	for (int i = 0; i < ACTION_MAX; i++)
	{
		AnimIndex[i] = MV1AttachAnim(modelhandle, i, -1, false);
		AnimTotalTime[i] = MV1GetAttachAnimTotalTime(modelhandle, AnimIndex[i]);
	}
	MV1SetRotationXYZ(modelhandle, VGet(0.f,0.f, 0.0f));
	//顔のテクスチャのindexを取得
	textureIndex = MV1GetMaterialDifMapTexture(modelhandle, 1);
}
Player::~Player()
{
	//delete _hit;
	//delete _rope;
	_modelmgr->ModelIdAllDelete();
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
	gravity();

	_tmpPos.x = _pos.x;
	_tmpPos.y = _pos.y;
	if (feverFlag == true) {
	//	FeverGravity();

		FeverUpdata(input);
	}
	else if (feverFlag == false) {
		//重力
	//	gravity();
		//移動制御
		setMove(input);
	}
	//ｽﾃｰﾀｽ制御
	setState();
	HitToEnemy();		//敵と当たったとき
	//ﾌｨｰﾊﾞｰﾃﾞｰﾀ受け取り
	GetFeverData();
}

//移動系の処理
void Player::setMove(Input* input)
{
	setDir(input);
	moveJump();
	//setState();
	moveWall();
	moveRope();
	moveCrouch(input);
	accelePL();
	EnterDoor();
}
//ｽﾃｰﾀｽ系の処理
void Player::setState(void)
{
	stFever();
	if (inviFlag)
	{
		_state = ST_INVINCIBLE;
	}
	stInvincible();
	if (/*_state != ST_FEVER*/feverFlag == false) {
		stVanish();
	}
}
void Player::FeverUpdata(Input* input)
{
	setDir(input);
	//FeverJump();
	//FeverWall();
	moveCrouch(input);               
	//moveFever();
	//壁抜けを一時的に見なかったことにします
	moveJump();
	moveWall();
	accelePL();

	moveRope();

	EnterDoor();
}
//向きを決める
void Player::setDir(Input* input)
{
	//ﾛｰﾌﾟ状態なら向きは変えられない
	float angle = static_cast<int>((RadAngle(_inpInfo.L_Stick.lstick) * 100.f));

	if (_state != ST_ROPE  && _state != ST_CROUCH) {
		//右
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_RIGHT;
			_tmpDir = DIR_RIGHT;
			if(_state != ST_JUMP)
			_state = ST_MOVE;
			airFlag = false;

			modelDirAngle = AngleRad(-90.f);
		}
		//左
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_LEFT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_LEFT;
			_tmpDir = DIR_LEFT;
			if (_state != ST_JUMP)
			_state = ST_MOVE;
			airFlag = false;

			modelDirAngle = AngleRad(90.f);
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
		else if (!_inpInfo.key.keybit.R_LEFT_BUTTON || !_inpInfo.key.keybit.R_RIGHT_BUTTON ||
			!_inpInfo.key.keybit.R_UP_BUTTON || !_inpInfo.key.keybit.R_DOWN_BUTTON ||
			input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_RIGHT || input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_LEFT ||
			input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_UP || input->GetStickDir(_inpInfo.L_Stick.lstick) != SD_DOWN) {
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
//	DrawFormatString(80, 260, 0xffffff, "%f", angle);
#endif
}
//移動制御
bool Player::accelePL(void)
{
	InputSetMove();
	//マップとの当たり判定
	//2ドットほど判定を狭めている
	//右
	Position2 nextPosRight[3];
	//右下	
	nextPosRight[0].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[0].y = _pos.y + (_plRect.h - 1);
	//右上
	nextPosRight[1].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[1].y = _pos.y;
	//右真ん中
	nextPosRight[2].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[2].y = _pos.y + (_plRect.h/2);
	for (int j = 0; j < 3; j++) {
		//登れる壁と登れない壁、ギミックとの判定
		if (_map->GetChipType(nextPosRight[j]) == CHIP_N_CLIMB_WALL
			|| _map->GetChipType(nextPosRight[j]) == CHIP_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosRight[j]) && (_hit->GimmickHitType(nextPosRight[j]) != GIM_FALL) && _hit->GimmickHitType(nextPosRight[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}

	//
	//左
	Position2 nextPosLeft[3];
	//左下
	nextPosLeft[0].x = _pos.x + vx + 2;
	nextPosLeft[0].y = _pos.y + (_plRect.h - 1);
	//左上
	nextPosLeft[1].x = _pos.x + vx + 2;
	nextPosLeft[1].y = _pos.y;
	//左真ん中
	nextPosLeft[2].x = _pos.x + vx + 2;
	nextPosLeft[2].y = _pos.y + (_plRect.h/2);
	for (int j = 0; j < 3; j++) {
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
					if (fMoveRight == true) {
						vx += ACCEL_X;
					}
					if (vx > MAX_SPEED) {
						vx = MAX_SPEED;
					}
				}
				else if (_dir == DIR_LEFT)
				{
					if (fMoveLeft == true) {
						vx -= ACCEL_X;
					}
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
			if (fMoveRight == true) {
				vx += ACCEL_X;
			}
			if (vx > MAX_SPEED) {
				vx = MAX_SPEED;
			}
		}
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
			if (fMoveLeft == true) {
				vx -= ACCEL_X;
			}
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
		JumpFlag = false;
	}
	int count = 0;
	//壁登り状態
	//操作性に難あり
	Position2 nextPos[6];
	//右下
	nextPos[0] = _wallRect.RightBottom();
	//左下
	nextPos[1] = _wallRect.LeftBottom();
	//右上
	nextPos[2] = _wallRect.RightTop();
	//左上
	nextPos[3] = _wallRect.LeftTop();
	//右真ん中
	nextPos[4] = _wallRect.RightTop();
	nextPos[4].y += _wallRect.h / 2;
	//左真ん中
	nextPos[5] = _wallRect.LeftTop();
	nextPos[5].y += _wallRect.h / 2;
	//ﾌﾟﾚｲﾔｰの下、マップチップ1分下
	Position2 downPos;
	downPos.x = _pos.x + (_plRect.w / 2);
	downPos.y = _pos.y + _plRect.h + MAP_CHIP_SIZE_Y +15;
	//downPos.x = _wallRect.Left() + (_wallRect.w/2);
	//downPos.y = _wallRect.Bottom() + MAP_CHIP_SIZE_Y;
	Position2 downPos2;
	downPos2.x = _pos.x + (_plRect.w/2);
	downPos2.y = _pos.y + _plRect.h + (MAP_CHIP_SIZE_Y/2)+10;
	//downPos2.x = _wallRect.Left() + (_wallRect.w / 2);
	//downPos2.y = _wallRect.Bottom() + (MAP_CHIP_SIZE_Y / 2);
	//壁登り状態にする条件
	for (int j = 0; j < 6; j++) {
		if ((_map->GetChipType(nextPos[j]) == CHIP_CLIMB_WALL ||_hit->GimmickHitType(nextPos[j]) == GIM_ATTRACT)&&  crouchFlag ==false) {
			count = 0;
			//壁が近くにあったとき、ボタンを押すと壁に張り付く
			if (_inpInfo.num >= 1)
			{
				if (WallFlag == false) {
					if (_dir == DIR_UP) {
						WallFlag = true;
						break;
					}
				}
			}
			else {
				if (WallFlag == false) {
					if (_key.keybit.R_UP_BUTTON) {
						WallFlag = true;
						break;
					}
				}
			}
			//もし足元に床がなければそのまま壁に張り付く
			if (_map->GetChipType(downPos) == CHIP_BLANK &&_map->GetChipType(downPos2) == CHIP_BLANK) {
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
	Position2 WallPosMiddl[2], WallPosTop[2], WallPosBottom[2];
	//右（真ん中）
	WallPosMiddl[0].x = _pos.x + _plRect.w;
	WallPosMiddl[0].y = _pos.y + (_plRect.h / 2);
	//WallPosMiddl[0].x = _wallRect.Right();
	//WallPosMiddl[0].y =_wallRect.Top()+ (_wallRect.h / 2);
	//左（真ん中）
	WallPosMiddl[1].x = _pos.x;
	WallPosMiddl[1].y = _pos.y + (_plRect.h / 2);
	//WallPosMiddl[1].x = _wallRect.Left();
	//WallPosMiddl[1].y = _wallRect.Top() +(_wallRect.h / 2);
	//右上
	WallPosTop[0].x = _pos.x + _plRect.w;
	WallPosTop[0].y = _pos.y;
	//WallPosTop[0] = _wallRect.RightTop();
	//左上
	WallPosTop[1].x = _pos.x;
	WallPosTop[1].y = _pos.y;
	//WallPosTop[0] = _wallRect.LeftTop();
	//補正のために下も確認する
	//右下
	WallPosBottom[0].x = _pos.x + _plRect.w;
	WallPosBottom[0].y = _pos.y + (_plRect.h - 1);
	//WallPosBottom[0].x = _wallRect.Right();
	//WallPosBottom[0].y = _wallRect.Bottom() - 1;
	//左下
	WallPosBottom[1].x = _pos.x;
	WallPosBottom[1].y = _pos.y + (_plRect.h - 1);
	//WallPosBottom[1].x = _wallRect.Left();
	//WallPosBottom[1].y = _wallRect.Bottom() -1;
	for (int j = 0; j < 2; j++) {
		//ｷｬﾗの半分以上,上はいけないようにする
		if (_rope->GetRopeState() != ST_ROPE_READY) {
			moveFlag = false;
			break;
		}
		if (_map->GetChipType(WallPosMiddl[j]) == CHIP_CLIMB_WALL
			|| _map->GetChipType(WallPosTop[j]) != CHIP_BLANK
			|| _hit->GimmickHitType(WallPosMiddl[j]) == GIM_ATTRACT) {
			moveFlag = true;
			break;
		}
		else {
			moveFlag = false;
		}
	}
	if (_map->GetChipType(WallPosTop[0]) == CHIP_CLIMB_WALL || _map->GetChipType(WallPosMiddl[0]) == CHIP_CLIMB_WALL) {
		if (WallFlag == true)
			fMoveRight = false;
	}
	else {
		fMoveRight = true;
	}
	if (_map->GetChipType(WallPosTop[1]) == CHIP_CLIMB_WALL || _map->GetChipType(WallPosMiddl[1]) == CHIP_CLIMB_WALL) {
		if (WallFlag == true)
			fMoveLeft = false;
	}
	else
	{
		fMoveLeft = true;
	}
	//半分以上で壁に張り付いてしまったときは半分まで下げる
	Position2 offsetPos[2];
	offsetPos[0].x = WallPosMiddl[0].x;
	offsetPos[0].y = WallPosMiddl[0].y + 3.0f;
	offsetPos[1].x = WallPosMiddl[1].x - 1.0f;
	offsetPos[1].y = WallPosMiddl[1].y + 3.0f;
	if (_state == ST_WALL) {
		if (moveFlag == false) {

			for (int f = 0; f < 2; f++) {
				if (_map->GetChipType(offsetPos[f]) != CHIP_CLIMB_WALL
					&& _map->GetChipType(WallPosBottom[f]) == CHIP_CLIMB_WALL) {
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
					WallAnimationFlag = true;
				}
				else if (_dir == DIR_DOWN || _key.keybit.L_DOWN_BUTTON)
				{
					vy = WALL_SPEED;
					WallAnimationFlag = true;
				}
				else {
					vy = 0.0f;
					WallAnimationFlag = false;
				}
			}
			else {
				if (_inpInfo.key.keybit.R_UP_BUTTON) {
					vy = -WALL_SPEED;
					WallAnimationFlag = true;

				}
				else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {
					vy = WALL_SPEED;
					WallAnimationFlag = true;

				}
				else {
					vy = 0.0f;
					WallAnimationFlag = false;
				}
			}
		}
		else if (_rope->GetRopeState() != ST_ROPE_READY) {

		}
		else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {		//キーボード
			vy = WALL_SPEED;
			WallAnimationFlag = true;

		}
		else if (_dir == DIR_DOWN) {
			vy = WALL_SPEED;
			WallAnimationFlag = true;

		}
		else {
			vy = 0.0f;
			WallAnimationFlag = false;
		}
		//下が地面だった時は止まる
		Position2 nextPosDown;
		nextPosDown.x = _pos.x + (_plRect.w / 2);
		nextPosDown.y = _pos.y + vy + (_plRect.h - 1);
		//nextPosDown = _wallRect.LeftBottom();
		//nextPosDown.x += _wallRect.w / 2;
		//nextPosDown.y += vy;
		if (_map->GetChipType(nextPosDown) == CHIP_CLIMB_WALL
			|| _map->GetChipType(nextPosDown) == CHIP_N_CLIMB_WALL
		 || _hit->GimmickHitType(nextPosDown) == GIM_ATTRACT) {
			vy = 0.0f;
		}
		//上が壁だったときは止まる
		Position2 nextPosUp;
		nextPosUp.x = _pos.x + (_plRect.w / 2);
		nextPosUp.y = _pos.y + vy;
		//nextPosUp = _wallRect.LeftTop();
		//nextPosUp.x += _wallRect.h / 2;
		//nextPosUp.y += vy;
		if (_map->GetChipType(nextPosUp) == CHIP_CLIMB_WALL
			|| _map->GetChipType(nextPosUp) == CHIP_N_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUp) == GIM_ATTRACT) {
			vy = 0.0f;
		}

		//位置補正
		Position2 tmpPos, WallPosDownL, WallPosDownR;
		//右下
		WallPosDownR.x = _pos.x + _plRect.w;
		WallPosDownR.y = _pos.y + (_plRect.h - 1);
		//WallPosDownR = _wallRect.RightBottom();
		//左下
		WallPosDownL.x = _pos.x;
		WallPosDownL.y = _pos.y + (_plRect.h - 1);
		//WallPosDownL = _wallRect.LeftBottom();

		tmpPos.y = (_pos.y - _plRect.h / 2) / 32 * 32;
		//tmpPos.y = (_pos.y - _plRect.h/2 -3);
		//tmpPos.y =(_wallRect.Top() -_wallRect.h );
		//moveFlagがfalseのときは位置補正を行う
		if (!moveFlag) {
			if (_rope->GetRopeState() != ST_ROPE_READY) {

			}
			else if (_inpInfo.num >= 1)
			{		//パッドの場合
				if (_dir == DIR_RIGHT || _dir==DIR_UP) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownR) == GIM_ATTRACT /*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x += 7;
						WallFlag = false;
					}
				}
				if (_dir == DIR_LEFT || _dir == DIR_UP) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownL) == GIM_ATTRACT /*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x -= 7;
						WallFlag = false;
					}
				}
			}
			else {	//キーボードの場合
				if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !_lastKey.keybit.R_RIGHT_BUTTON ||_inpInfo.key.keybit.R_UP_BUTTON&& !_lastKey.keybit.R_UP_BUTTON) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL ||_hit->GimmickHitType(WallPosDownR) == GIM_ATTRACT/* && !(_hit->GimmickHit(*this)&& _hit->GimmickHitType(*this) ==GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x += 7;
						WallFlag = false;
					}
				}
				if (_inpInfo.key.keybit.R_LEFT_BUTTON && !_lastKey.keybit.R_LEFT_BUTTON || _inpInfo.key.keybit.R_UP_BUTTON && !_lastKey.keybit.R_UP_BUTTON) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL  ||_hit->GimmickHitType(WallPosDownL) == GIM_ATTRACT/*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x -= 7;
						WallFlag = false;
					}
				}
			}
		}
		_pos.y += vy;
	}

#ifdef _DEBUG
	//DrawFormatString(10, 430, 0xffffff, "flag::%d", moveFlag);
#endif // _DEBUG
	return false;
}
void Player::FeverWall()
{
	//float vy;
	if (WallFlag == true) {
		_state = ST_WALL;
		JumpFlag = false;
	}
	int count = 0;
	//壁登り状態
	//操作性に難あり
	Position2 nextPos[6];
	//右下
	nextPos[0] = _wallRect.RightBottom();
	//左下
	nextPos[1] = _wallRect.LeftBottom();

	//右上
	nextPos[2] = _wallRect.RightTop();
	//左上
	nextPos[3] = _wallRect.LeftTop();
	//右真ん中
	nextPos[4] = _wallRect.RightTop();
	nextPos[4].x += 5;
	nextPos[4].y += _wallRect.h / 2;
	//左真ん中
	nextPos[5] = _wallRect.LeftTop();
	nextPos[5].x -= 5;
	nextPos[5].y += _wallRect.h / 2;
	//ﾌﾟﾚｲﾔｰの下、マップチップ1分下
	Position2 downPos;
	downPos.x = _pos.x + (_wallRect.w / 2);
	downPos.y = _pos.y + _wallRect.h + MAP_CHIP_SIZE_Y;
	Position2 downPos2;
	downPos2.x = _pos.x + (_wallRect.w / 2);
	downPos2.y = _pos.y + _wallRect.h + (MAP_CHIP_SIZE_Y / 3);

	//壁登り状態にする条件
	for (int j = 0; j < 6; j++) {
		if ((_map->GetChipType(nextPos[j]) == CHIP_CLIMB_WALL || _hit->GimmickHitType(nextPos[j]) == GIM_ATTRACT) && crouchFlag == false) {
			count = 0;
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
			if (_map->GetChipType(downPos) == CHIP_BLANK&&_map->GetChipType(downPos2) == CHIP_BLANK) {
				if (WallFlag == false) {
					WallFlag = true;
					vx = 0;
				}
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
	Position2 WallPosMiddl[2], WallPosTop[2], WallPosBottom[2];
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
	WallPosBottom[0].y = _pos.y + (_plRect.h - 1);
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
			|| _map->GetChipType(WallPosTop[j]) != CHIP_BLANK
			|| _hit->GimmickHitType(WallPosMiddl[j]) == GIM_ATTRACT) {
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
	offsetPos[1].x = WallPosMiddl[1].x - 1.0f;
	offsetPos[1].y = WallPosMiddl[1].y + 3.0f;
	if (_state == ST_WALL) {
		if (moveFlag == false) {

			for (int f = 0; f < 2; f++) {
				if (_map->GetChipType(offsetPos[f]) != CHIP_CLIMB_WALL
					&& _map->GetChipType(WallPosBottom[f]) == CHIP_CLIMB_WALL) {
					_pos.y++;
				}
				else {
					continue;
				}
			}
		}
	}
	//位置補正
	Position2 tmpPos, WallPosDownL, WallPosDownR;
	//右下
	WallPosDownR.x = _pos.x + _wallRect.w;
	WallPosDownR.y = _pos.y + (_wallRect.h - 1);
	//左下
	WallPosDownL.x = _pos.x;
	WallPosDownL.y = _pos.y + (_wallRect.h - 1);

	tmpPos.y = (_pos.y - _wallRect.h / 2) / 32 * 32 +3;
	if (_state == ST_WALL) {
		//壁の中で移動可能なら
		if (moveFlag) {
			if (_inpInfo.num >= 1)
			{
				if (_dir == DIR_UP || _key.keybit.L_UP_BUTTON)
				{
					vy = -WALL_SPEED;
					WallAnimationFlag = true;

				}
				else if (_dir == DIR_DOWN || _key.keybit.L_DOWN_BUTTON)
				{
					vy = WALL_SPEED;
					WallAnimationFlag = true;

				}
				else {
					vy = 0.0f;
					WallAnimationFlag = false;

				}
			}
			else {
				if (_inpInfo.key.keybit.R_UP_BUTTON) {
					vy = -WALL_SPEED;
					WallAnimationFlag = true;
				}
				else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {
					vy = WALL_SPEED;
					WallAnimationFlag = true;
				}
				else {
					vy = 0.0f;
					WallAnimationFlag = false;

				}
			}
		}
		else if (_rope->GetRopeState() != ST_ROPE_READY) {

		}
		else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {		//キーボード
			vy = WALL_SPEED;
			WallAnimationFlag = true;
		}
		else if (_dir == DIR_DOWN) {
			vy = WALL_SPEED;
			WallAnimationFlag = true;
		}
		else {
			vy = 0.0f;
			WallAnimationFlag = false;
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
		nextPosUp.x = _pos.x + (_plRect.w / 2);
		nextPosUp.y = _pos.y + vy;
		if (_map->GetChipType(nextPosUp) == CHIP_CLIMB_WALL
			|| _map->GetChipType(nextPosUp) == CHIP_N_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUp) == GIM_ATTRACT) {
			vy = 0.0f;
		}

		//moveFlagがfalseのときは位置補正を行う
		if (!moveFlag && WallFlag == true) {
			if (_rope->GetRopeState() != ST_ROPE_READY) {

			}
			else if (_inpInfo.num >= 1)
			{		//パッドの場合
				if (_dir == DIR_RIGHT || _dir == DIR_UP) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownR) == GIM_ATTRACT /*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x += 7;
						WallFlag = false;
					}
				}
				if (_dir == DIR_LEFT || _dir == DIR_UP) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownL) == GIM_ATTRACT /*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x -= 7;
						WallFlag = false;
					}
				}
			}
			else {	//キーボードの場合
				if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !_lastKey.keybit.R_RIGHT_BUTTON || _inpInfo.key.keybit.R_UP_BUTTON && !_lastKey.keybit.R_UP_BUTTON) {
					//右下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownR) == GIM_ATTRACT /*&& !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x += 7;
						WallFlag = false;
					}
				}
				if (_inpInfo.key.keybit.R_LEFT_BUTTON && !_lastKey.keybit.R_LEFT_BUTTON || _inpInfo.key.keybit.R_UP_BUTTON && !_lastKey.keybit.R_UP_BUTTON) {
					//左下が登れる壁だったら補正する
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL || _hit->GimmickHitType(WallPosDownL) == GIM_ATTRACT/* && !(_hit->GimmickHit(*this) && _hit->GimmickHitType(*this) == GIM_ATTRACT)*/) {
						_pos.y = tmpPos.y;
						_pos.x -=7;
						WallFlag = false;
					}
				}
			}
		}

		_pos.y += vy;
	}		if (_map->GetChipType(WallPosTop[0]) == CHIP_CLIMB_WALL || _map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL) {
			if (WallFlag == true)
				fMoveRight = false;
		}
		else {
			fMoveRight = true;
		}
		if (_map->GetChipType(WallPosTop[1]) == CHIP_CLIMB_WALL || _map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL) {
			if (WallFlag == true)
				fMoveLeft = false;
		}
		else
		{
			fMoveLeft = true;
		}
}
//ﾛｰﾌﾟ状態の処理
bool Player::moveRope(void)
{
	SoundMgr& so = SoundMgr::Instance();
	//ﾛｰﾌﾟ状態なら動けない
	if (_rope->GetRopeState() != ST_ROPE_READY) {
		_state = ST_ROPE;
		ropeFlag = true;
		if (vx != 0 && JumpFlag) {
			moveRopeJumpFlag = true;
		}
		if (moveFlag == true) {
			moveFlag = false;
		}
		vx = 0.0f;
	//	so.BgmStart("Bgm/extend.mp3", SCENE_GAME); // 用編集
	}
	else {
		AnimNowTime[ACTION_TONGUE_SET] = 0.0f;
		AnimNowTime[ACTION_TONGUE_GO] = 0.0f;
		ropeFlag = false;
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
	if (_state == ST_MOVE || _state == ST_JUMP || _state == ST_ROPE || vy != 0 
		||ropeFlag == true || tmpFlag != crouchFlag) {
		vanCnt = 60 * VANISH_CNT;
		vanFlag = false;
		alfa = 255;
		deathFlag = true;
	}

	if (vanCnt <= 0) {
	//	_state = ST_VANISH;
		vanFlag = true;
		deathFlag = false;
	}
#ifdef _DEBUG
	//DrawFormatString(0, 120, 0xffffff, "%d", vanCnt);
#endif
	tmpFlag = crouchFlag;
	return false;
}
void Player::moveCrouch(Input* input)
{
	int tmpPos = 10;
	if (crouchFlag == true) {
		_state = ST_CROUCH;
	}
	if (_inpInfo.num >= 1) {
		if (_dir == DIR_DOWN) {
			crouchFlag = true;
		}
		if( input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_UP &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL){
			crouchFlag = false;
		}
	}
	else {

		if (_key.keybit.R_DOWN_BUTTON && !_lastKey.keybit.R_DOWN_BUTTON) {
			if (WallFlag == false && JumpFlag == false && ropeFlag == false) {
				if (crouchFlag == true) {
					crouchFlag = false;
				}
				else if (crouchFlag == false) {
					crouchFlag = true;
				}
				else {}
			}
		}
		if (_key.keybit.R_UP_BUTTON && !_lastKey.keybit.R_UP_BUTTON) {
			//上キーでしゃがみを解除
			if (WallFlag == false && JumpFlag == false && ropeFlag == false) {
				if (crouchFlag == true) {
					crouchFlag = false;
				}
			}

		}
	}
	if (_state == ST_CROUCH) {
		//_pos.y = _pos.y + tmpPos;

		if (moveFlag == true) {
			moveFlag = false;
		}
		vx = 0.0f;
		//ジャンプしたらしゃがみを解除
		if (JumpFlag == true) {
			//_pos.y -= 18;
			crouchFlag = false;
		}
	}
	else {
		AnimNowTime[ACTION_CROUCH] = 0.0f;
	}
}
//ﾌｨｰﾊﾞｰ処理
bool Player::stFever(void)
{
	//とりあえずﾌｨｰﾊﾞｰ
	if (keyData[KEY_INPUT_Z]) {
		if (_fd.feverCnt > 0) {

			if (feverFlag == false) {
				feverFlag = true;
				_fd.feverCnt--;
				GameMain::Instance().SetFeverData(_fd);
			}
		}
	}
	if (feverFlag == true) {
		if (_state == ST_ROPE) {
			feverTime = feverTime;
		}
		else {
			//_state = ST_FEVER;
			deathFlag = false;
			feverTime--;
		}
	}
	if (feverTime < 0) {
		if (plPlaceCheck() == false)
		{
			helpFever = true;
			feverTime = -1;
		}
		else {
			helpFever = false;
			feverFlag = false;
			feverTime = 60 * FEVER_CNT;
		}
		if (plPlaceCheck() == true) {
			feverFlag = false;
			feverTime = 60 * FEVER_CNT;
		}
	}

#ifdef _DEBUG
	//DrawFormatString(600, 10, 0xffffff, "%d", feverTime);
#endif
	return false;
}
//無敵状態の処理
void Player::stInvincible(void)
{
	if (inviFlag)
	{
		if (_state == ST_INVINCIBLE)
		{
			if (--inviCnt < 0)
			{
				_state = ST_DEF;
				inviCnt = INVINCIBLETIMER * 60;
				inviFlag = false;
			}
		}
	}
}
//ｼﾞｬﾝﾌﾟ処理
bool Player::moveJump(void)
{
	SoundMgr& so = SoundMgr::Instance();
	if (_state == ST_ROPE && crouchFlag == true) {
		JumpFlag = false;
		return;
	}
	//ｼﾞｬﾝﾌﾟ
	if (JumpFlag == false) {
		if (_inpInfo.num >= 1) {
			if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
				vy = -JUMP_POWER;
				JumpFlag = true;
				airFlag = true;
				so.SeStart("Se/question.mp3", SCENE_GAME);

			}
		}
		else {
			if (keyData[KEY_INPUT_SPACE] ^ oldkeyData[KEY_INPUT_SPACE] & keyData[KEY_INPUT_SPACE]) {
				vy = -JUMP_POWER;
				JumpFlag = true;
				airFlag = true;
				so.SeStart("Se/question.mp3", SCENE_GAME);

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
	Position2 nextPosDown[2];
	nextPosDown[0].x = _pos.x + (_plRect.w -2);
	nextPosDown[0].y = _pos.y + (vy/2) + (_plRect.h +2);
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy/2) + (_plRect.h +2);
	//登れる壁、登れない壁との判定
	for (int j = 0; j < 2; j++) {
		if (_map->GetChipType(nextPosUP[j]) == CHIP_N_CLIMB_WALL
			|| _map->GetChipType(nextPosUP[j]) == CHIP_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUP[j]) == GIM_ATTRACT) {
			vy = 0.0f;
			break;
		}
	}

	if (_state == ST_JUMP) {
		for (int j = 0; j < 2; j++) {
			if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
				|| _map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL
				|| _hit->GimmickHitType(nextPosDown[j]) == GIM_ATTRACT) {
				vy = 0.0f;
				airFlag = false;
				_state = ST_MOVE;
				break;
			}
		}
	}
	if(JumpFlag == true){
		if (_rope->GetRopeState() != ST_ROPE_SELECT)
		{
			airCnt++;
			if (airCnt >= 120) {
				_pos.y = _pos.y + 10;
				JumpFlag = false;
				_state = ST_DEF;
				airCnt = 0;
			}
		}

	}
	else {
		airCnt = 0;
	}
	//flagがtrueならｼﾞｬﾝﾌﾟ状態
	if (JumpFlag == true&& airFlag ==true) {
		_state = ST_JUMP;
	}
	return false;
}
void Player::FeverJump()
{
	SoundMgr& so = SoundMgr::Instance();
	//ｼﾞｬﾝﾌﾟ
	if (JumpFlag == false) {
		if (_inpInfo.num >= 1) {
			if (_key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
				vy = -JUMP_POWER;
				JumpFlag = true;
				so.SeStart("Se/question.mp3", SCENE_GAME);
			}
		}
		else {
			if (keyData[KEY_INPUT_SPACE] ^ oldkeyData[KEY_INPUT_SPACE] & keyData[KEY_INPUT_SPACE]) {
				vy = -JUMP_POWER;
				JumpFlag = true;
				so.SeStart("Se/question.mp3", SCENE_GAME);
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
	if (_state == ST_JUMP) {
		if (_rope->GetRopeState() != ST_ROPE_SELECT)
		{
			airCnt++;
			if (airCnt >= 60) {
				_pos.y = _pos.y + 2;
				JumpFlag = false;
				airCnt = 0;
			}
		}
	}
	else {
		airCnt = 0;
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
	//足元判定
	Position2 nextPosDown[2];
	//右下
	nextPosDown[0].x = _pos.x + (_plRect.w - 2);
	nextPosDown[0].y = _pos.y + (_plRect.h - 2) + (vy / 2);
	//左下
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (_plRect.h - 2) + (vy / 2);
	for (int j = 0; j < 2; j++) {
		if (_map->GetChipType(nextPosUP[j]) == CHIP_N_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUP[j]) == GIM_ATTRACT
			|| (_map->GetChipType(nextPosUP[j]) == CHIP_CLIMB_WALL&&_map->GetChipType(nextPosDown[j]) == CHIP_BLANK)) {
			vy = 0.0f;
			break;
		}
	}
	if (_state == ST_JUMP) {
		for (int j = 0; j < 2; j++) {
			if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
				|| _map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL
				|| _hit->GimmickHitType(nextPosDown[j]) == GIM_ATTRACT) {
				vy = 0.0f;
				airFlag = false;
				_state = ST_MOVE;
				break;
			}
		}
	}
	if (JumpFlag == true) {
		if (_rope->GetRopeState() != ST_ROPE_SELECT)
		{
			airCnt++;
			if (airCnt >= 120) {
				_pos.y = _pos.y + 10;
				JumpFlag = false;
				_state = ST_DEF;
				airCnt = 0;
			}
		}

	}
	else {
		airCnt = 0;
	}

	//flagがtrueならｼﾞｬﾝﾌﾟ状態
	if (JumpFlag == true && airFlag == true) {
		_state = ST_JUMP;
	}
}
//敵と当たった時の処理を行う
void Player::HitToEnemy()
{
	SoundMgr& so = SoundMgr::Instance();
	GameMain& gm = GameMain::Instance();
	
	if (_hit->EnemyHit(*this)) {
		if (_state != ST_INVINCIBLE && feverFlag ==false)
		{
			if (deathFlag == true) {
				_state = ST_DETH;
				so.SeStart("Se/PlHit.mp3", SCENE_GAME);

				//完全敗北
				if (gm.GetResultData().life < 0)
				{
					_state = ST_OVER;
				}
			}
			else if (deathFlag == false) {
				//死なない
			}
		}
		else {
		}
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
			airFlag = false;
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
			//JumpFlag = true;
			airFlag = true;
		}
	}
	//ﾛｰﾌﾟ状態ならうごけない
	if (_state == ST_ROPE ) {
		vy = 0.0f;
	}
	//加速度を足す
	//速度調整のため２で割っている
	_pos.y += (int)vy / 2.0f;
}
//フィーバー時の重力です
void Player::FeverGravity()
{

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
	nextPosDown2[0].y = nextPosDown[0].y - 8;
	//左下
	nextPosDown2[1].x = nextPosDown[1].x;
	nextPosDown2[1].y = nextPosDown[1].y - 8;
	//真ん中
	nextPosDown2[2].x = nextPosDown[2].x;
	nextPosDown2[2].y = nextPosDown[2].y - 8;
	//壁登り状態なら重力は無視
	if (_state == ST_WALL) {
		return;
	}
	if (JumpFlag == true && vy > 0) {
		for (int j = 0; j < 3; j++) {
			if (_map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL && (_map->GetMapNum(nextPosDown[j]) != _map->GetMapNum(nextPosDown2[j]))) {
				vy = 0.0f;
				JumpFlag = false;
				break;
			}
		}
	}
	if (JumpFlag == true && vy < 0) {

	}
	//登れない壁との判定
	for (int j = 0; j < 3; j++) {
		//下に地面があれば重力加算をやめる
		if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosDown[j]) && _hit->GimmickHitType(nextPosDown[j]) != GIM_FALL && _hit->GimmickHitType(nextPosDown[j]) != GIM_DOOR)) {
			vy = 0.0f;
			JumpFlag = false;
			airFlag = false;
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
			JumpFlag = true;			//つけてないと上床すり抜け判定がうまくいかないっぽいです
			airFlag = true;
		}
	}

	//ﾛｰﾌﾟ状態ならうごけない
	if (_state == ST_ROPE ) {
		vy = 0.0f;
	}
	//加速度を足す
	//速度調整のため２で割っている
	_pos.y += (int)vy / 2.0f;
}
bool Player::plPlaceCheck()
{
	if (_map->GetChipType(_plRect.LeftTop()) != CHIP_BLANK
		|| _map->GetChipType(_plRect.LeftBottom()) != CHIP_BLANK
		|| _map->GetChipType(_plRect.RightTop()) != CHIP_BLANK
		|| _map->GetChipType(_plRect.RightBottom()) != CHIP_BLANK) {
		return false;
	}
	return true;
}
void Player::Draw(Position2& offset)
{
	//ワールド座標からスクリーン座標に変換した後のモデル表示用のposをセット
	WorldToScreenPos = ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_plRect.w / 2), _pos.y - offset.y + (_plRect.h),0.0f));
	//時機
	if (vanFlag == true) {
		//透過率をだんだん上げていく
		alfa = max(alfa - VANISH, tranceMax);
		//輪郭線をだんだん上げていく
		LineNum += 0.005f;
		if (LineNum >= 0.5f)
		{
			LineNum = 0.5f;
		}
	}
	else {
		LineNum = 0.0f;
		switch (_state)
		{
			//ｽﾃﾙｽ状態
		//case ST_VANISH:
		//	//透過率をだんだん上げていく
		//	alfa = max(alfa - VANISH, tranceMax);
		//	//DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x  + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0xff0000, true);
		//	break;
			//ﾛｰﾌﾟ状態
		case ST_ROPE:
			LineNum = 0.0f;
			alfa = 255;
			break;
			//壁登り状態
		case ST_WALL:
			LineNum = 0.0f;
			alfa = 255;
			break;
			//ﾌｨｰﾊﾞｰ状態
		case ST_FEVER:
			LineNum = 0.5f;
			alfa = 50;
			break;
		case ST_INVINCIBLE:
			LineNum = 0.0f;
			if (inviCnt % 10/2 == 0){
				alfa = 255;
			}
			else {
				//完全に消す
				alfa = 0;
			}
			break;
		default:
			break;
		}
		if (feverFlag == true) {
			LineNum = 0.5f;
			alfa = 50;
		}
	}
		_plRect.SetCenter(_pos.x + (_plRect.w / 2), _pos.y + (_plRect.h / 2));
		_wallRect.SetCenter(_pos.x + (_plRect.w / 2), _pos.y + ((_plRect.h / 4) * 3) - 1);

	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, modelDirAngle, 0.f));
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, WorldToScreenPos);
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(1.4f, 1.4f, 1.4f));
	//モデルの透過率の設定
	MV1SetOpacityRate(modelhandle, alfa / 255.f);

	//アニメーション切り替え
	AnimationSwitching();

	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(modelhandle, LineNum);

#ifdef _DEBUG
		//_plRect.Draw(offset);
		//_wallRect.Draw(offset,0xffffff);
#endif
}
//Rect取得
Rect& Player::GetRect()
{
	return _plRect;
}
//ｽﾃｰﾀｽ取得
CHAR_ST Player::GetcharState(void)
{
	if (feverFlag == true) {
		return ST_FEVER;
	}
	return _state;
}
//_pos取得
Position2& Player::GetPos(void)
{
	//return Position2(_pos.x,_pos.y);
	return _tmpPos;
}
//向き取得
DIR Player::GetDir(void)
{
	return _dir;
}
DIR Player::GetTmpDir()
{
	return _tmpDir;
}
//初期位置に戻す
void Player::SetInitPos()
{
	_pos = initPos;
	//加速度も元に戻す
	vx = 0.0f;
	vy = 0.0f;
	alfa = 255;
	_state = ST_INVINCIBLE;//無敵にする
	AnimNowTime[ACTION_KNOCKBACK] = 0.0f;
	AnimNowTime[ST_OVER] = 0.0f;
	feverFlag = false;
	feverTime = 60 * FEVER_CNT;
	inviFlag = true;
	inviCnt = INVINCIBLETIMER * 60;
}
//初期位置をセットする
void Player::SetInitPos(Position2 p)
{
	_pos = Position3(p.x,p.y-(_plRect.h/2)+6,0.f);	//yに足しているのは最初の座標で埋まらないようにするため
	initPos = _pos;
	_tmpPos.x = _pos.x;
	_tmpPos.y = _pos.y;
}
bool Player::EnterDoor()
{
	if (_hit->GimmickEnter(*this)) {
		//if(CheckHitKey(KEY_INPUT_A)){
		if (GameMain::Instance().GetResultData().midFlag == true) {
			return true;
		}
	}
	return false;
}
void Player::SetRetryPos(Position2 midPos)
{
	_pos = Position3(midPos.x,midPos.y - (_plRect.h / 2),0.f);
	_tmpPos.x = _pos.x;
	_tmpPos.y = _pos.y;

	//加速度も元に戻す
	vx = 0.0f;
	vy = 0.0f;
	_state = ST_INVINCIBLE;
	alfa = 255;
	feverFlag = false;
	feverTime = 60 * FEVER_CNT;
	inviFlag = true;
	inviCnt = INVINCIBLETIMER * 60;
}

//ポースから用
void Player::SetInitPausePos()
{
	_pos = initPos;
	_tmpPos.x = _pos.x;
	_tmpPos.y = _pos.y;
	//加速度も元に戻す
	vx = 0.0f;
	vy = 0.0f;
	alfa = 255;
	_state = ST_DEF;
	AnimNowTime[ACTION_KNOCKBACK] = 0.0f;
	AnimNowTime[ST_OVER] = 0.0f;
	feverFlag = false;
	feverTime = 60 * FEVER_CNT;
}

Position2 Player::ReturnWoToScPos2ver()
{
	return Position2(WorldToScreenPos.x, WorldToScreenPos.y);
}

bool Player::GetStateCrouch()
{
	return crouchFlag;
}
bool Player::GetStateVanish()
{
	return vanFlag;
}
bool Player::GetStateRope()
{
	return ropeFlag;
}

//プレイヤーの状態によってアニメーションを切り替えている関数です
//テクスチャも変えています
void Player::AnimationSwitching(void)
{
	ImageMgr& im = ImageMgr::Instance();
	switch (_state)
	{
		//通常状態
	case ST_DEF:
	case ST_STOP:
	case ST_INVINCIBLE:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("player_model/face.png", SCENE_TITLE), FALSE);
		AnimationManager(ACTION_WAIT, ANIMATION_SPEED_DEF,0.0f);
		break;
		//移動状態
	case ST_MOVE:
		AnimationManager(ACTION_WALK, ANIMATION_SPEED_SUPER,0.0f);
		break;
		//ﾛｰﾌﾟ状態
	case ST_ROPE:
		//発射準備中はのけぞるアニメーション
		if (_rope->GetRopeState() != ST_ROPE_READY && _rope->GetRopeState() == ST_ROPE_SELECT)
		{
			AnimationManager(ACTION_TONGUE_SET, ANIMATION_SPEED_HIGH, AnimTotalTime[ACTION_TONGUE_SET]);
		}
		//舌発射!!!!!!
		else if (_rope->GetRopeState() != ST_ROPE_READY && _rope->GetRopeState() != ST_ROPE_SELECT)
		{
			AnimationManager(ACTION_TONGUE_GO, ANIMATION_SPEED_HIGH, AnimTotalTime[ACTION_TONGUE_GO]);
		}
		break;
		//壁登り状態
	case ST_WALL:
		AnimationManager(ACTION_CLIMB, ANIMATION_SPEED_HIGH,0.0f);
		break;
		//ジャンプ状態
	case ST_JUMP:
		AnimationManager(ACTION_JUMP, ANIMATION_SPEED_DEF, 0.0f);
		break;
		//リスポーンダウン状態
	case ST_DETH:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("player_model/cryFace.png", SCENE_TITLE), FALSE);
		AnimationManager(ACTION_KNOCKBACK, ANIMATION_SPEED_DEF, AnimTotalTime[ACTION_KNOCKBACK]);
		break;
		//ゲームオーバー状態
	case ST_OVER:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("player_model/surprisFace.png", SCENE_TITLE), FALSE);
		AnimationManager(ACTION_DAMAGE, ANIMATION_SPEED_DEF, AnimTotalTime[ACTION_DAMAGE]);
		break;
	case ST_CROUCH:
		//しゃがみ状態
		AnimationManager(ACTION_CROUCH, ANIMATION_SPEED_DEF, AnimTotalTime[ACTION_CROUCH]);
		break;

	default:
		break;
	}
}
void Player::GetFeverData()
{
	_fd = GameMain::Instance().ReturnFeverData();
}

//第一引数 アタッチするアニメーション名
//第二引数 アニメーションを進めるスピード
//第三引数 アニメーションをループさせるかどうか(今のところ0.0f指定でループ,AnimTotalTime[アクション]で1回再生)
//AnimTotalTime[アクション]で指定した場合必ずどこかしらの処理でAnimNowTime[]を0にする
void Player::AnimationManager(PLAYER_ACTIONS actions, float animspeed,float looptime)
{
	for (int i = 0; i <= ACTION_MAX; i++)
	{
		if (AnimIndex[i] == actions)
		{
			continue;
		}
		//不要なアニメーションのブレンド率を0にする
		MV1SetAttachAnimBlendRate(modelhandle, AnimIndex[i], 0.0f);
	}
	//適用するアニメーションの設定 ブレンド率を1.0fに
	MV1SetAttachAnimBlendRate(modelhandle, AnimIndex[actions], 1.0f);
	//アニメーションをアタッチ
	MV1SetAttachAnimTime(modelhandle, AnimIndex[actions], AnimNowTime[actions]);

	//指定アニメーションのフレームを進める
	//アクションクライムに関しては別の場所でカウントを進める
	if (actions != ACTION_CLIMB)
	{
		AnimNowTime[actions] += animspeed;
	}
	else 
	{
		if(WallAnimationFlag) 
			AnimNowTime[actions] += animspeed;
		else {
		}
	}
	//現在のアニメーションが最大フレームまでいったらループする
	if (AnimNowTime[actions] >= AnimTotalTime[actions])
	{
		AnimNowTime[actions] = looptime;
	}
}