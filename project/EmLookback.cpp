#include "DxLib.h"
#include "Geometry.h"
#include "EmLookback.h"
#include "MapCtl.h"
#include "HitClass.h"
#include "EnemyServer.h"
#include "Player.h"
#include "Rope.h"
#include "ModelMgr.h"


EmLookback::EmLookback(Position2 pos, Player& pl, Rope& rope, EnemyServer& server,HitClass& hit) :_player(pl), _rope(rope), _server(server),_hit(hit)
{
	//_hit = new HitClass();
	//_player = new Player();
	_map = MapCtl::GetInstance();
	_modelmgr = ModelMgr::Instance();
	_emRect.w = 32;
	_emRect.h = 64;
	_pos.x = pos.x;
	_pos.y = pos.y -32;//���W���}�b�v�`�b�v����̍���œo�^����Ă���̂ŁA���W���烌�N�g�T�C�Y�|�}�b�v�`�b�v�T�C�Y����
	_initPos = _pos;
	_dir = DIR_RIGHT;

	_emEye.pos.x = _pos.x;
	_emEye.pos.y = _pos.y + (_emRect.h / 4);
	_emEye.r = 40;
	_emType = ENEMY_TURN;
	_state = EM_ST_MOVE;

	vx = 0.0f;
	vy = 0.0f;
	upAngle = 120;
	downAngle = 60;
	returnFlag = false;
	emSpeed = 1;
	LookCount = 0;
	FearCount = 180;
	loseSightCnt = 180;

	_tmpOffset.x = 0;
	_tmpOffset.y = 0;
	//�̃f�[�^������
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData._level = ALERT_LEVEL_1;

	modelhandle = _modelmgr->ModelIdReturn("Enemy_model/teki2.pmx", SCENE_RESULT);
	ETexture = LoadGraph("Enemy_model/teki2-1.png");
	textureIndex = MV1GetMaterialDifMapTexture(modelhandle, 0);
	modelDirAngle = 0.0f;
}

EmLookback::~EmLookback()
{
	//delete _hit;
}

void EmLookback::Updata()
{
	SetRange();
	_emData.lookRange = _emEye;
	_individualData.midFlag = _server.SendMidFlag();
	if (returnFlag == true) {
		ReturnPoint();
	}
	else {
		SetMove();
	}

	Visibility();

	Gravity();
}

void EmLookback::Draw(Position2 offset)
{
	//���f���̉�]�p�x�̐ݒ�(���W�A��)
	MV1SetRotationXYZ(modelhandle, VGet(0.0f, modelDirAngle, 0.0f));
	//���f����pos��ݒ�+���[���h���W����X�N���[���֕ϊ�
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_emRect.w / 2), _pos.y - offset.y + (_emRect.h), 0)));
	//���f���̊g��k���l�̐ݒ�
	MV1SetScale(modelhandle, VGet(3.f, 3.f, 3.f));

	//if (_commonData.midFlag)
	//{
		//�e�N�X�`����ύX
		MV1SetTextureGraphHandle(modelhandle, textureIndex, ETexture, FALSE);
	//}

	//���f����֊s��0.0f�ŕ`�� 
	_modelmgr->Draw(modelhandle, 0.0f);

	switch (_state)
	{
	case EM_ST_NONE:
	case EM_ST_MOVE:
		//DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0xff0000, true);
		break;
	case EM_ST_DIS:
		//DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0x0000ff, true);
		break;
	case EM_ST_RETURN:
		break;
	case EM_ST_RE_DIS:
		break;
	case EM_ST_FEAR:
		//DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y, (int)_pos.x - offset.x + _emRect.w, (int)_pos.y - offset.y + _emRect.h, 0x00ff00, true);
		break;
	default:
		break;
	}
	_tmpOffset = offset;
	_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
	if (_state != EM_ST_FEAR) {
		if (_dir == DIR_RIGHT) {
			modelDirAngle = AngleRad(-90.0f);
			_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			DrawCircleGauge(_emEye.Center().x - offset.x, _emEye.Center().y - offset.y, 33.3, vigiImage[_individualData._level], 16.6);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		}
		else if (_dir == DIR_LEFT) {
			modelDirAngle = AngleRad(90.0f);
			_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			DrawCircleGauge(_emEye.Center().x - offset.x, _emEye.Center().y - offset.y, 83.3, vigiImage[_individualData._level], 66.6);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		}
	}
	returnDir(offset);
	_emRect.SetCenter(_pos.x + (_emRect.w / 2), _pos.y  +(_emRect.h / 2));
	//_emEye.Draw(offset);

#ifdef _DEBUG
	//_emRect.Draw(offset);
	//DrawFormatString(10, 380, 0xffffff, "�U��Ԃ�:%d", LookCount);
#endif 
}
void EmLookback::SetMove()
{
	vx = 0;
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {
		setDir();
	}
	else if(_state == EM_ST_DIS || _state == EM_ST_RE_DIS){
		//�ǂ������Ă��铮��
		if (_individualData.plFoundFlag == true) {
			LookPl();
		}
		else if (_individualData.dataSendFlag == true) {
			LoseSight();
		}
		else{}
	}
	else if (_state == EM_ST_FEAR) {
		//���݂̓���
		moveFear();
	}
	else {
		setDir();		//��Ԃ̗�O
	}
	LimitMove();
}
void EmLookback::setDir(void)
{
	if (_state == EM_ST_MOVE) {
		//3�b���ƂɌ������ς��
		if (LookCount < EM_LOOKBACK_TIME) {
			LookCount++;
			if (LookCount == EM_LOOKBACK_TIME) {
				LookCount *= -1;
			}
		}
		if (LookCount < 0) {
			_dir = DIR_RIGHT;
			//_emEye.SetCenter(_pos.x  + _emRect.w, _pos.y  + (_emRect.h / 4), _emEye.r);
		}
		else {
			_dir = DIR_LEFT;
			//_emEye.SetCenter(_pos.x , _pos.y + (_emRect.h / 4), _emEye.r);
		}
	}
	else {
		if (_dir == DIR_RIGHT) {
			modelDirAngle = AngleRad(-90.0f);
			_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
		}
		else if (_dir == DIR_LEFT) {
			modelDirAngle = AngleRad(90.0f);
			_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
		}
		LookCount = 0;
	}
}
void EmLookback::Visibility()
{
	_emData.lookDir = _dir;
	//���E����(�v���C���[���������Ƃ�)
	if (_state == EM_ST_MOVE || _state == EM_ST_RETURN) {

		if (_hit.EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
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
		if (_hit.EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
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
		if (_hit.EnemyViewing(_emData, _player.GetRect()) && _player.GetcharState() != ST_VANISH) {
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
	Position2 nextRightPos;
	nextRightPos.x = _pos.x + _emRect.w;
	nextRightPos.y = _pos.y + (_emRect.h / 2);
	Position2 nextLeftPos;
	nextLeftPos.x = _pos.x;
	nextLeftPos.y = _pos.y + (_emRect.h / 2);
	//�v���C���[����������ǂ������Ă���
	//���͌����Ă�������ɓ����悤�ɂ��Ă���
	if (_state == EM_ST_DIS || _state == EM_ST_RE_DIS) {
		if (_dir == DIR_LEFT) {
			if (_map->GetChipType(nextLeftPos) == CHIP_BLANK||
				_map->GetChipType(nextLeftPos) != CHIP_CLIMB_WALL&&
				_map->GetChipType(nextLeftPos) != CHIP_N_CLIMB_WALL
				&& _hit.GimmickHitType(nextLeftPos) != GIM_ATTRACT) {
				vx -= emSpeed;
			}
		}
		else if (_dir == DIR_RIGHT) {
			if (_map->GetChipType(nextRightPos) == CHIP_BLANK ||
				_map->GetChipType(nextRightPos) != CHIP_CLIMB_WALL &&
				_map->GetChipType(nextRightPos) != CHIP_N_CLIMB_WALL&&
				 _hit.GimmickHitType(nextRightPos) != GIM_ATTRACT) {
				vx += emSpeed;
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
	//DrawFormatString(10, 450, 0xffffff, "����:%d", FearCount);
#endif
}
void EmLookback::EnemyFalter()
{
	if (_state != EM_ST_FEAR) {
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_hit.IsHit(GetRect(), _rope.GetCircle())) {
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
			returnFlag = true;
		}
	}
}
void EmLookback::LimitMove()
{
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
	_pos.x += vx;
}
void EmLookback::ReturnPoint()
{
	Position2 nextRightPos;
	nextRightPos.x = _pos.x + _emRect.w;
	nextRightPos.y = _pos.y + (_emRect.h / 2);
	Position2 nextLeftPos;
	nextLeftPos.x = _pos.x;
	nextLeftPos.y = _pos.y + (_emRect.h / 2);
	if (_pos.x - _initPos.x >= 20) {	//�G�̂ق���init���E���ɂ���
		if (_map->GetChipType(nextLeftPos) == CHIP_BLANK ||
			_map->GetChipType(nextLeftPos) != CHIP_CLIMB_WALL&&
			_map->GetChipType(nextLeftPos) != CHIP_N_CLIMB_WALL&& 
			_hit.GimmickHitType(nextLeftPos) != GIM_ATTRACT) {

			_dir = DIR_LEFT;
			_pos.x -= emSpeed;
		}
	}
	else if (_pos.x - _initPos.x <= -20) {
		if (_map->GetChipType(nextRightPos) == CHIP_BLANK ||
			_map->GetChipType(nextRightPos) != CHIP_CLIMB_WALL &&
			_map->GetChipType(nextRightPos) != CHIP_N_CLIMB_WALL&&
			_hit.GimmickHitType(nextRightPos) != GIM_ATTRACT) {

			_dir = DIR_RIGHT;
			_pos.x += emSpeed;
		}
	}
	else {
		returnFlag = false;
	}
}
//�d�͂ɂ���
void EmLookback::Gravity()
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
void EmLookback::SetRange()
{
	_individualData._level = _server.AlertLevel();
	if (_individualData._level == ALERT_LEVEL_1) {
		_emEye.r = 40;
	}
	else if (_individualData._level == ALERT_LEVEL_2) {
		_emEye.r = 60;
	}
	else if (_individualData._level == ALERT_LEVEL_3) {
		_emEye.r = 80;
	}
	else {
		_emEye.r = 40;
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
//��Ԃ�Ԃ�
ENEMY_STATE& EmLookback::GetState()
{
	return _state;
}
void EmLookback::GetClass(HitClass * hit, Player & pl)
{
	//_hit = hit;
	_player = pl;
}
void EmLookback::SetInitPos()
{
	_pos = _initPos;
	_individualData.dataSendFlag = false;
	_individualData.plFoundFlag = false;
	_individualData._level = ALERT_LEVEL_1;
}
//�I�t�Z�b�g�̈׌����Ă���������m�F���܂�
void EmLookback::returnDir(Position2 offset)
{
	if (_state == EM_ST_MOVE) {
		if (returnFlag == false) {

			if (LookCount < 0) {
				_dir = DIR_RIGHT;
				_emEye.SetCenter(_pos.x + _emRect.w, _pos.y + (_emRect.h / 4), _emEye.r);
			}
			else {
				_dir = DIR_LEFT;
				_emEye.SetCenter(_pos.x, _pos.y + (_emRect.h / 4), _emEye.r);
			}
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