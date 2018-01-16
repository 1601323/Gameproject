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
	speed = 1;				//�����X�s�[�h�ݒ�
	moveFlag = false;

	dis = 0;
	interCnt = 0;
	fearCnt = 180;
	loseSightCnt = 180;
	//�̃f�[�^������
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
//���������������΂Ȃ������̂ł�����move�̊Ǘ������Ă��܂�
void EmAround::Move()
{	
	CheckMove();
	//�ʏ��Ԃ̏ꍇ
	if (_state != EM_ST_FEAR&&_individualData._level == ALERT_LEVEL_1) {
		//_state = EM_ST_MOVE;
	}
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {
		if (moveFlag == false) {
			BasicMove();		//���E�֓����܂�
		}
		else if (moveFlag == true) {
			InterMove();		//���΂炭�����~�܂�
		}
		else {
		}
	}
	//��ڲ԰����������
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_individualData.plFoundFlag == true) {
			FoundMove();
		}
		else if (_individualData.dataSendFlag == true) {
			LoseSight();
		}
	}
	//��ڲ԰�����������Ƃ�
	//if(_state == EM_ST_DIS )
	//���ݏ�Ԃ̎�
	if (_state == EM_ST_FEAR) {
	moveFear();

	}
}
void EmAround::BasicMove()
{
	//�m�F�̂��߂Ɉꎞ�R�����g�A�E�g
	//�ŏ��͂Ƃ肠�����ǂꂭ�炢���������甽�Α��ɓ����悤�ɂ���
	//dis++;
	//if (dis % 70 ==0) {
	//	//lookBackFlag = !lookBackFlag;
	//	moveFlag = true;
	//}

	if (_dir == DIR_RIGHT) {		//�E
		_pos.x += speed;
	}
	else if (_dir == DIR_LEFT) {	//��
		_pos.x -= speed;
	}
}
//�U��Ԃ�O�̓���ɂ���
void EmAround::InterMove()
{
	if (_state == EM_ST_MOVE) {	//��ڲ԰����������Ă��Ȃ��A���N���A��������������Ă��Ȃ��Ƃ�
		interCnt++;
		//�Ƃ肠�����P�b�Ԓ�~�����Ĕ��Α��Ɉړ�������
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
//��ڲ԰�������̓����ɂ���
void EmAround::FoundMove()
{
	//���i�K�ł͎��E�ɓ����Ă���Ƃ������ǂ�������

	//��ڲ԰�̂ق����E�ɂ�����
	if (_pl.GetPos().x >= _pos.x) {
		_pos.x += speed;
	}
	else {
		_pos.x -= speed;
	}
}
//�}�b�v�Ƃ̂����蔻��ɂԂ����Ă��Ȃ���
void EmAround::CheckMove()
{
	moveFlag = false;
	//���E�̈ړ��𔻒肷��
	Position2 nextLeftPos;		//���g�̍����𔻒肷��
	nextLeftPos.x = _pos.x - speed;
	nextLeftPos.y = _pos.y + (_emRect.h / 2);
	Position2 nextRightPos;
	nextRightPos.x = _pos.x + (_emRect.w) + speed;
	nextRightPos.y = _pos.y + (_emRect.h/2);
	//���E�n�ʂ̔�����s��
	Position2 nextLeftDown;
	nextLeftDown.x = _pos.x - speed;
	nextLeftDown.y = _pos.y + (_emRect.h);
	Position2 nextRightDown;
	nextRightDown.x = _pos.x + (_emRect.w) + speed;
	nextRightDown.y = _pos.y + (_emRect.h);

	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN){	//��������Ԃł����
		if (_dir == DIR_LEFT) {		//�����ɓ����Ă���Ƃ�
			if (_map->GetChipType(nextLeftPos) == CHIP_N_CLIMB_WALL ||
				_map->GetChipType(nextLeftPos) == CHIP_CLIMB_WALL ) {	//�����ǂł����
				moveFlag = true;	//�����𔽓]������	
			}
			else if (_map->GetChipType(nextLeftDown) != CHIP_N_CLIMB_WALL &&
				_map->GetChipType(nextLeftDown) != CHIP_CLIMB_WALL) {	//�����̑����𔻒肷��
				moveFlag = true;
			}
			else {	//���������蔲���Ă��܂����Ƃ�
			}
		}
		else {		//�E���ɓ����Ă���Ƃ�
			if (_map->GetChipType(nextRightPos) == CHIP_N_CLIMB_WALL ||
				_map->GetChipType(nextRightPos) == CHIP_CLIMB_WALL) {	//�E���ǂł����												
				moveFlag = true;	//�����𔽓]������
			}
			else if (_map->GetChipType(nextRightDown) != CHIP_N_CLIMB_WALL &&
				_map->GetChipType(nextRightDown) != CHIP_CLIMB_WALL) {
				moveFlag = true;
			}
			else {	//���������蔲���Ă��܂����Ƃ�
			}
		}
	}
}
//���E�ɂ���
void EmAround::Visibility()
{
	_emData.lookAngle = 60;
	_emData.lookDir = _dir;
	_emData.lookRange = _emEye;
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {

		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "����", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "���G��", 0xffffff);
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_ALERT || _state == EM_ST_RE_ALERT) {
		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "����", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "���G��", 0xffffff);
			if (_individualData.plFoundFlag == true) {
				LoseSight();
			}
		}
	}
	else if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_hit->EnemyViewing(_emData, _pl.GetRect()) && _pl.GetcharState() != ST_VANISH) {
			DrawString(100, 300, "����", 0xffffff);
			_state = EM_ST_DIS;
			_individualData.plFoundFlag = true;
		}
		else {
			DrawString(100, 300, "���G��", 0xffffff);
			_individualData.plFoundFlag = false;
			_individualData.dataSendFlag = true;
		}
	}
}
//��ڲ԰�����������Ƃ��̓���
void EmAround::LoseSight()
{
	//����낫��낷��ƌ�����������������Ǝv��
	//�ʏ��Ԃɖ߂�O��Enemy�Ɍ������t���O�𑗂�
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
			DrawString(100, 100, "�G�ɓ���������I", 0xffffff);
			_state = EM_ST_FEAR;
		}
		else {
			DrawString(100, 100, "������", 0xffffff);
		}
	}
}
//۰�߂ɓ����������̏���
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
//�d�͂ɂ���
void EmAround::Gravity()
{
	Position2 nextPosDown[3];
	//�E
	nextPosDown[0].x = _pos.x + (_emRect.w - 2);
	nextPosDown[0].y = _pos.y + (vy / 2) + (_emRect.h);
	//��
	nextPosDown[1].x = _pos.x + 2;
	nextPosDown[1].y = _pos.y + (vy / 2) + (_emRect.h);
	//���S
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