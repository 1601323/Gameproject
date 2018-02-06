
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
	speed = 1;				//�����X�s�[�h�ݒ�
	moveFlag = false;
	ModelDirChangeFlag = false;

	dis = 0;
	interCnt = 0;
	fearCnt = 180;
	loseSightCnt = 180;
	//�̃f�[�^������
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData.midFlag = false;
	_individualData._level = ALERT_LEVEL_1;
	_rangeLevel = RANGE_1;
	midFlag = false;
	//���f���ǂݍ���
	modelhandle = _modelmgr->ModelIdReturn("Enemy_model/teki.pmx", SCENE_RESULT);
	textureIndex = MV1GetMaterialDifMapTexture(modelhandle, 0);
	//�����p�x
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
//���������������΂Ȃ������̂ł�����move�̊Ǘ������Ă��܂�
void EmAround::Move()
{	
	GameMain& gm = GameMain::Instance();
	vx = 0;
	if (midFlag == false) {
		if (gm.GetResultData().midFlag == true) {
			midFlag = true;
		}
	}
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
	CheckMove();
	_pos.x += vx;
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

	//if (_state == EM_ST_MOVE) {
	//	speed = 1;
	//}
	//else if (_state == EM_ST_RETURN) {
	//	speed = 2;
	//}
	//else { speed = 1; }
	speed = midFlag ? 2 : 1;
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
		ModelDirChangeFlag = true;
		//�Ƃ肠�����P.5�b�Ԓ�~�����Ĕ��Α��Ɉړ�������

		//�����[��ƐU������Ă܂�
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
//��ڲ԰�������̓����ɂ���
void EmAround::FoundMove()
{
	//���i�K�ł͎��E�ɓ����Ă���Ƃ������ǂ�������
	speed = midFlag ? 3 : 2;
	//��ڲ԰�̂ق����E�ɂ�����
	if (_pl.GetPos().x >= _pos.x) {
		vx += speed;
	}
	else {
		vx -= speed;
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
	//���E�Ŕ���������Ȃ�
	Position2  LeftViewPos;
	LeftViewPos.x = _pos.x - speed - (_emEye.r / 2);
	LeftViewPos.y = _pos.y + (_emRect.h/2);
	Position2 RightViewPos;
	RightViewPos.x = _pos.x + (_emRect.w) + speed + (_emEye.r / 2);
	RightViewPos.y = _pos.y + (_emRect.h/2);
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
				_map->GetChipType(nextLeftPos) == CHIP_CLIMB_WALL	||
				_map->GetChipType(LeftViewPos) == CHIP_CLIMB_WALL	||
				_map->GetChipType(LeftViewPos) == CHIP_N_CLIMB_WALL ||
				(_hit.GimmickHit(nextLeftPos) && _hit.GimmickHitType(nextLeftPos) == GIM_ATTRACT)) {	//�����ǂł����
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
				_map->GetChipType(nextRightPos) == CHIP_CLIMB_WALL	 ||
				_map->GetChipType(RightViewPos) == CHIP_CLIMB_WALL	 ||
				_map->GetChipType(RightViewPos) == CHIP_N_CLIMB_WALL ||
				(_hit.GimmickHit(nextRightPos) && _hit.GimmickHitType(nextRightPos) == GIM_ATTRACT)) {	//�E���ǂł����												
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
	//�ǂƂ̂����蔻��
	if (_state == EM_ST_RE_DIS || _state == EM_ST_DIS) {
		Position2 nextMove[2];
		//����
		nextMove[0].x = _pos.x + vx;
		nextMove[0].y = _pos.y + (_emRect.h / 2);
		//�E��
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
//���E�ɂ���
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
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&
			(_hit.IsHit(GetRect(), _rope.GetCircle()) || (_hit.IsHit(GetRect(), _rope.GetCircle2())))) {
#ifdef _DEBUG
			//DrawString(100, 100, "�G�ɓ���������I", 0xffffff);
#endif
			_state = EM_ST_FEAR;
		}
		else {
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
	ImageMgr& im = ImageMgr::Instance();
	//���f���̉�]�p�x�̐ݒ�(���W�A��)
	MV1SetRotationXYZ(modelhandle, VGet(0.0f, modelDirAngle, 0.0f));
	//���f����pos��ݒ�+���[���h���W����X�N���[���֕ϊ�
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_emRect.w / 2),_pos.y - offset.y + (_emRect.h),0)));
	//���f���̊g��k���l�̐ݒ�
	MV1SetScale(modelhandle,VGet(3.f,3.f,3.f));
	//�e�N�X�`����ύX
	MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("Enemy_model/teki-2.png", SCENE_RESULT), FALSE);
	//���f����֊s��0.0f�ŕ`�� 
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
	//�T�C�Y�͉�
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