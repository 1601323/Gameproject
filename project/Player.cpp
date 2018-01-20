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
	vanCnt = 60 * VANISH_CNT;	//�Ƃ肠�����R�b
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
//�X�V���ꂽHitClass���󂯎�邽�߂̊֐��ł�
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
		//�d��
		gravity();
		//�ړ�����
		setMove(input);
	}
	//�ð������
	setState();
	//HitToEnemy();		//�G�Ɠ��������Ƃ�
}

//�ړ��n�̏���
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

//�ð���n�̏���
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
//���������߂�
void Player::setDir(Input* input)
{
	//۰�ߏ�ԂȂ�����͕ς����Ȃ�
	float angle = static_cast<int>((RadAngle(_inpInfo.L_Stick.lstick) * 100.f));

	if (_state != ST_ROPE) {
		//�E
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON  ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_RIGHT;
			_state = ST_MOVE;
		}
		//��
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON ||
			(input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_LEFT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_dir = DIR_LEFT;
			_state = ST_MOVE;
		}
		//��
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
			//�����ĂȂ�
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
//�ړ�����
bool Player::accelePL(void)
{
	InputSetMove();
	//�}�b�v�Ƃ̓����蔻��
	//2�h�b�g�قǔ�������߂Ă���
	//�E
	Position2 nextPosRight[2];
	//�E��	
	nextPosRight[0].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[0].y = _pos.y + (_plRect.h - 1);
	//�E��
	nextPosRight[1].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//�o���ǂƓo��Ȃ��ǁA�M�~�b�N�Ƃ̔���
		if (_map->GetChipType(nextPosRight[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosRight[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosRight[j]) && (_hit->GimmickHitType(nextPosRight[j]) != GIM_FALL)&&_hit->GimmickHitType(nextPosRight[j])!= GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	
	//
	//��
	Position2 nextPosLeft[2];
	//����
	nextPosLeft[0].x = _pos.x + vx + 2;
	nextPosLeft[0].y = _pos.y + (_plRect.h - 1);
	//����
	nextPosLeft[1].x = _pos.x + vx + 2;
	nextPosLeft[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//�o���ǂƓo��Ȃ��ǁA�M�~�b�N�Ƃ̔���
		if (_map->GetChipType(nextPosLeft[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosLeft[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosLeft[j]) && _hit->GimmickHitType(nextPosLeft[j]) != GIM_FALL&&_hit->GimmickHitType(nextPosLeft[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	//�����x�𑫂�
	_pos.x += (int)vx;
	return false;
}
void Player::InputSetMove()
{
	//�L�[�{�[�h�ƃp�b�h�ňړ������𕪂��Ă��܂�
	//�ړ�(�Q�[���p�b�h)
	if (_inpInfo.num >= 1)
	{
		//������L_SensingFlag�����܂��X�V����Ă��Ȃ��Ƃ݂�!! (error��)
		//���͂����ςȂ��ŃR���p�C������Ɠ����܂�
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
		//�ړ�(�L�[�{�[�h)
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
//�ǈړ��̏���
bool Player::moveWall(void)
{
	if (WallFlag == true) {
		_state = ST_WALL;
	}
	int count =0;
	//�Ǔo����
	//���쐫�ɓ��
	Position2 nextPos[4];
	//�E��
	nextPos[0].x = _pos.x  + _plRect.w;
	nextPos[0].y = _pos.y + (_plRect.h - 1);
	//����
	nextPos[1].x = _pos.x ;
	nextPos[1].y = _pos.y + (_plRect.h - 1);
	//�E��
	nextPos[2].x = _pos.x + _plRect.w;
	nextPos[2].y = _pos.y;
	//����
	nextPos[3].x = _pos.x;
	nextPos[3].y = _pos.y;
	//��ڲ԰�̉��A�}�b�v�`�b�v1����
	Position2 downPos;
	downPos.x = _pos.x + (_plRect.w / 2);
	downPos.y = _pos.y + _plRect.h+ MAP_CHIP_SIZE_Y;
	//�Ǔo���Ԃɂ������
	for (int j = 0; j < 4; j++) {
		if (_map->GetChipType(nextPos[j]) == CHIP_CLIMB_WALL) {
			count = 0;
			//�ǂ��߂��ɂ������Ƃ��A�{�^���������ƕǂɒ���t��
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
			//���������ɏ����Ȃ���΂��̂܂ܕǂɒ���t��
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
	//�ǂ̈ړ�����
	Position2 WallPosMiddl[2],WallPosTop[2],WallPosBottom[2];
	//�E�i�^�񒆁j
	WallPosMiddl[0].x = _pos.x + _plRect.w;
	WallPosMiddl[0].y = _pos.y + (_plRect.h / 2);
	//���i�^�񒆁j
	WallPosMiddl[1].x = _pos.x;
	WallPosMiddl[1].y = _pos.y + (_plRect.h / 2);
	//�E��
	WallPosTop[0].x = _pos.x + _plRect.w;
	WallPosTop[0].y = _pos.y;
	//����
	WallPosTop[1].x = _pos.x;
	WallPosTop[1].y = _pos.y;
	//�␳�̂��߂ɉ����m�F����
	//�E��
	WallPosBottom[0].x = _pos.x + _plRect.w;
	WallPosBottom[0].y = _pos.y + (_plRect.h-1);
	//����
	WallPosBottom[1].x = _pos.x;
	WallPosBottom[1].y = _pos.y + (_plRect.h - 1);
	for (int j = 0; j < 2; j++) {
		//��ׂ̔����ȏ�,��͂����Ȃ��悤�ɂ���
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
	//�����ȏ�ŕǂɒ���t���Ă��܂����Ƃ��͔����܂ŉ�����
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
		//�ǂ̒��ňړ��\�Ȃ�
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
		else if (_inpInfo.key.keybit.R_DOWN_BUTTON) {		//�L�[�{�[�h
			vy = WALL_SPEED;
		}
		else if (_dir == DIR_DOWN){
			vy = WALL_SPEED;
		}
		else {
			vy = 0.0f;
		}


		//�����n�ʂ��������͎~�܂�
		Position2 nextPosDown;
		nextPosDown.x = _pos.x + (_plRect.w / 2);
		nextPosDown.y = _pos.y + vy + (_plRect.h - 1);
		if (_map->GetChipType(nextPosDown) == CHIP_CLIMB_WALL
		 || _map->GetChipType(nextPosDown) == CHIP_N_CLIMB_WALL) {
			vy = 0.0f;
		}
		//�オ�ǂ������Ƃ��͎~�܂�
		Position2 nextPosUp;
		nextPosUp.x = _pos.x + (_plRect.w/2);
		nextPosUp.y = _pos.y + vy;
		if (_map->GetChipType(nextPosUp) == CHIP_CLIMB_WALL
			|| _map->GetChipType(nextPosUp) == CHIP_N_CLIMB_WALL
			|| _hit->GimmickHitType(nextPosUp) == GIM_ATTRACT) {
			vy = 0.0f;
		}

		//�ʒu�␳
		Position2 tmpPos,WallPosDownL,WallPosDownR;
		//�E��
		WallPosDownR.x = _pos.x + _plRect.w;
		WallPosDownR.y = _pos.y + (_plRect.h - 1);
		//����
		WallPosDownL.x = _pos.x;
		WallPosDownL.y = _pos.y + (_plRect.h - 1);

		tmpPos.y = (_pos.y - _plRect.h / 2) / 32 * 32;
		//moveFlag��false�̂Ƃ��͈ʒu�␳���s��
		if (!moveFlag) {
			if (_rope->GetRopeState() != ST_ROPE_READY) {

			}
			else if (_inpInfo.num >= 1)
			{		//�p�b�h�̏ꍇ
				if (_dir == DIR_RIGHT) {
					//�E�����o���ǂ�������␳����
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
				if (_dir == DIR_LEFT) {
					//�������o���ǂ�������␳����
					if (_map->GetChipType(WallPosDownL) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
			}
			else {	//�L�[�{�[�h�̏ꍇ
				if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !_lastKey.keybit.R_RIGHT_BUTTON) {
					//�E�����o���ǂ�������␳����
					if (_map->GetChipType(WallPosDownR) == CHIP_CLIMB_WALL) {
						_pos.y = tmpPos.y;
						WallFlag = false;
					}
				}
				if (_inpInfo.key.keybit.R_LEFT_BUTTON && !_lastKey.keybit.R_LEFT_BUTTON) {
					//�������o���ǂ�������␳����
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

//۰�ߏ�Ԃ̏���
bool Player::moveRope(void)
{

	//۰�ߏ�ԂȂ瓮���Ȃ�
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

	//�������E�����ɒ��n����
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
	//�E��	
	nextPosRight[0].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[0].y = _pos.y + (_plRect.h - 1);
	//�E��
	nextPosRight[1].x = _pos.x + vx + (_plRect.w - 2);
	nextPosRight[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//�o���ǂƓo��Ȃ��ǁA�M�~�b�N�Ƃ̔���
		if (_map->GetChipType(nextPosRight[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosRight[j]) && (_hit->GimmickHitType(nextPosRight[j]) != GIM_FALL) && _hit->GimmickHitType(nextPosRight[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}

	//
	//��
	Position2 nextPosLeft[2];
	//����
	nextPosLeft[0].x = _pos.x + vx + 2;
	nextPosLeft[0].y = _pos.y + (_plRect.h - 1);
	//����
	nextPosLeft[1].x = _pos.x + vx + 2;
	nextPosLeft[1].y = _pos.y;

	for (int j = 0; j < 2; j++) {
		//�o���ǂƓo��Ȃ��ǁA�M�~�b�N�Ƃ̔���
		if (_map->GetChipType(nextPosLeft[j]) == CHIP_N_CLIMB_WALL
			|| (_hit->GimmickHit(nextPosLeft[j]) && _hit->GimmickHitType(nextPosLeft[j]) != GIM_FALL&&_hit->GimmickHitType(nextPosLeft[j]) != GIM_DOOR)) {
			vx = 0.0f;
			break;
		}
	}
	//�����x�𑫂�
	_pos.x += (int)vx;

}
//��ٽ��Ԃ̏���
//3�b�����Ȃ������������
bool Player::stVanish(void)
{
	//3�b�������
	if (vanCnt > 0) {
		vanCnt--;
	}
	//�����Ă����綳�Ă�߂�
	//�Ǔo���Ԃœ����Ă������ٽ�ɂȂ�Ȃ�
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

//̨��ް����
bool Player::stFever(void)
{
	//�Ƃ肠����̨��ް
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

//�ެ��ߏ���
bool Player::moveJump(void)
{
	//flag��true�Ȃ�ެ��ߏ��
	if (JumpFlag == true) {
		_state = ST_JUMP;
	}
	//�ެ���
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
		//�������������邽�߂ɉ����x��MAX_SPEED��ݒ�
		//�ǂ��Ȃ�
		if (_state == ST_ROPE) {
			if (_inpInfo.key.keybit.R_RIGHT_BUTTON) {
				vx = MAX_SPEED;
			}
			if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
				vx = -MAX_SPEED;
			}
		}
	}

	//�}�b�v�Ƃ̔���
	//2�h�b�g�قǔ�������߂Ă���
	//�㕔�̓����蔻��
	Position2 nextPosUP[2];
	//�E��	
	nextPosUP[0].x = _pos.x + (_plRect.w - 2);
	nextPosUP[0].y = _pos.y + (vy / 2);
	//����
	nextPosUP[1].x = _pos.x + 2;
	nextPosUP[1].y = _pos.y + (vy / 2);
		//�o���ǁA�o��Ȃ��ǂƂ̔���
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
	//flag��true�Ȃ�ެ��ߏ��
	if (JumpFlag == true) {
		_state = ST_JUMP;
	}
	//�ެ���
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
		//�������������邽�߂ɉ����x��MAX_SPEED��ݒ�
		//�ǂ��Ȃ�
		if (_state == ST_ROPE) {
			if (_inpInfo.key.keybit.R_RIGHT_BUTTON) {
				vx = MAX_SPEED;
			}
			if (_inpInfo.key.keybit.R_LEFT_BUTTON) {
				vx = -MAX_SPEED;
			}
		}
	}
	//�}�b�v�Ƃ̔���
	//2�h�b�g�قǔ�������߂Ă���
	//�㕔�̓����蔻��
	Position2 nextPosUP[2];
	//�E��	
	nextPosUP[0].x = _pos.x + (_plRect.w - 2);
	nextPosUP[0].y = _pos.y + (vy / 2);
	//����
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
	//���@
	DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0xffffff, true);
	switch (_state)
	{
		//��ٽ���
	case ST_VANISH:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x  + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0xff0000, true);
		break;
		//۰�ߏ��
	case ST_ROPE:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x  + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0x00ffff, true);
		break;
		//�Ǔo����
	case ST_WALL:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y  + 32 -offset.y, 0xff00ff, true);
		break;
		//̨��ް���
	case ST_FEVER:
		DrawBox((int)_pos.x -offset.x, (int)_pos.y -offset.y, (int)_pos.x + 32 -offset.x, (int)_pos.y + 32 -offset.y, 0x0000ff, true);
		DrawString((int)_pos.x  - 20 -offset.x, (int)_pos.y  - 20 -offset.y, "�_FEVER�^", 0x0000ff);
		break;
	default:
		break;
	}
	_plRect.SetCenter(_pos.x + (_plRect.w / 2), _pos.y + (_plRect.h / 2));

#ifdef _DEBUG
	DrawString(400, 200, "�ԁF�X�e���X���", 0xffffff);
	DrawString(400, 220, "���F۰�ߎg�p���", 0xffffff);
	DrawString(400, 180, "L���۰ق�۰�ߎg�p�i���j", 0xffffff);
	DrawFormatString(10, 400, 0xffffff, "�ð���F%d", GetcharState());
	DrawFormatString(10, 415, 0xffffff, "dir:%d ��:2 �E:3", _dir);
	_plRect.Draw(offset);
#endif
}

//�G�Ɠ����������̏������s��
void Player::HitToEnemy()
{
	if (_hit->EnemyHit(*this)) {
		_state = ST_DETH;
	}
	else
	{
	}
}
//�d��
void Player::gravity(void)
{
	//�Ǔo���ԂȂ�d�͖͂���
	if (_state == ST_WALL) {
		return;
	}
	//�}�b�v�Ƃ̔���
	//2�h�b�g�قǔ�������߂Ă���
	Position2 nextPosDown[3];
	//�E��	
	nextPosDown[0].x = _pos.x + (_plRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_plRect.h);
	//����
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_plRect.h);
	//�^��
	nextPosDown[2].x = _pos.x + (_plRect.w / 2);
	nextPosDown[2].y = _pos.y + (vy / 2) + (_plRect.h);
	//�o���ǁA�o��Ȃ��ǂƂ̔���
	for (int j = 0; j < 3; j++) {
		if (_map->GetChipType(nextPosDown[j]) == CHIP_N_CLIMB_WALL
		 || _map->GetChipType(nextPosDown[j]) == CHIP_CLIMB_WALL
		 || (_hit->GimmickHit(nextPosDown[j]) && _hit->GimmickHitType(nextPosDown[j]) != GIM_FALL && _hit->GimmickHitType(nextPosDown[j]) != GIM_DOOR)) {
			vy = 0.0f;
			JumpFlag = false;
			break;
		}
		else {
			//�����ɉ����Ȃ�������d�͂𑫂�
			//���x�����̂���3�Ŋ����Ă���
			vy += GRAVITY / 3.0f;
			//�ꉞMax�l��ݒ肵�Ă���
			if (vy > MAX_GRAVITY) {
				vy = MAX_GRAVITY;
			}
			//�󒆂�������Ƃ肠�����ެ��ߏ��
			JumpFlag = true;
		}
	}
	//۰�ߏ�ԂȂ炤�����Ȃ�
	if (_state == ST_ROPE) {
		vy = 0.0f;
	}
	//�����x�𑫂�
	//���x�����̂��߂Q�Ŋ����Ă���
	_pos.y += (int)vy / 2.0f;
}
//�t�B�[�o�[���̏d�͂ł�
void Player::FeverGravity()
{
	//�Ǔo���ԂȂ�d�͖͂���
	if (_state == ST_WALL) {
		return;
	}
	//�}�b�v�Ƃ̔���
	//2�h�b�g�قǔ�������߂Ă���
	Position2 nextPosDown[3];
	//�E��	
	nextPosDown[0].x = _pos.x + (_plRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_plRect.h);
	//����
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_plRect.h);
	//�^��
	nextPosDown[2].x = _pos.x + (_plRect.w / 2);
	nextPosDown[2].y = _pos.y + (vy / 2) + (_plRect.h);
	//����̍�
	Position2 nextPosDown2[3];
	//�E��
	nextPosDown2[0].x = nextPosDown[0].x;
	nextPosDown2[0].y = nextPosDown[0].y - 2;
	//����
	nextPosDown2[1].x = nextPosDown[1].x;
	nextPosDown2[1].y = nextPosDown[1].y - 2;
	//�^��
	nextPosDown2[2].x = nextPosDown[2].x;
	nextPosDown2[2].y = nextPosDown[2].y - 2;
	//�o���ǁA�o��Ȃ��ǂƂ̔���
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
			//�����ɉ����Ȃ�������d�͂𑫂�
			//���x�����̂���3�Ŋ����Ă���
			vy += GRAVITY / 3.0f;
			//�ꉞMax�l��ݒ肵�Ă���
			if (vy > MAX_GRAVITY) {
				vy = MAX_GRAVITY;
			}
			//�󒆂�������Ƃ肠�����ެ��ߏ��
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
	//۰�ߏ�ԂȂ炤�����Ȃ�
	if (_state == ST_ROPE) {
		vy = 0.0f;
	}
	//�����x�𑫂�
	//���x�����̂��߂Q�Ŋ����Ă���
	_pos.y += (int)vy / 2.0f;
}
//Rect�擾
Rect& Player::GetRect()
{
	return _plRect;
}

//�ð���擾
CHAR_ST Player::GetcharState(void)
{
	return _state;
}

//_pos�擾
Position2& Player::GetPos(void)
{
	return _pos;
}
//�����擾
DIR Player::GetDir(void)
{
	return _dir;
}
//�����ʒu�ɖ߂�
void Player::SetInitPos()
{
	_pos = initPos;
	//�����x�����ɖ߂�
	vx = 0.0f;
	vy = 0.0f;
	_state = ST_DEF;
}
//�����ʒu���Z�b�g����
void Player::SetInitPos(Position2 p)
{
	_pos = p;
	initPos = _pos;
}
bool Player::EnterDoor()
{
	if (_hit->GimmickEnter(*this)) {
		if (_inpInfo.key.keybit.R_UP_BUTTON) {
			DrawString(30, 360, "�N���A������", GetColor(255, 255, 255));
			return true;
		}
	}
	return false;
}
void Player::SetRetryPos(Position2 midPos)
{
	_pos = midPos;
	//�����x�����ɖ߂�
	vx = 0.0f;
	vy = 0.0f;
	_state = ST_DEF;
}