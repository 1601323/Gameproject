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

//����������
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

	//���f���ǂݍ���
	modelhandle = _modelmgr->ModelIdReturn("Tongue_model/sita5.mv1", SCENE_RESULT);
	//���ꂼ��̃A�j���[�V�������A�^�b�`+�����Ԃ̐ݒ�
	AnimAttachIndex = MV1AttachAnim(modelhandle,1,-1,false);
	AnimTotalTime = MV1GetAttachAnimTotalTime(modelhandle, AnimAttachIndex);
	//0�`19 ��20
	for (int j = 0; j < ROPE_LENGTH_MAX; j++)
	{
		ropeinfo.push_back(j);//�擪��������
	}
}
//���[�v�`�揈��
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
	//�����������ǂ����ŃA�j���[�V������i�߂邩�߂�
	AnimNowTime = tongueHitTurn ? AnimNowTime -= 0.5f : AnimNowTime += 0.5f;
	//���݂̃A�j���[�V�������ő�t���[���܂ł������烋�[�v����
	if (AnimNowTime >= AnimTotalTime)
	{
		AnimNowTime = 0;
	}
	//���f���̉�]�p�x�̐ݒ�(���W�A��)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, RopeAngle_Y, RopeAngle_Z));
	//�A�j���[�V�������A�^�b�`
	MV1SetAttachAnimTime(modelhandle, AnimAttachIndex, AnimNowTime);
	//���f����pos��ݒ�
	MV1SetPosition(modelhandle, VGet(_player->ReturnWoToScPos2ver().x - mentenanceNum_X, _player->ReturnWoToScPos2ver().y - mentenanceNum_Y, 0));
	//���f���̊g��k���l�̐ݒ�
	MV1SetScale(modelhandle, VGet(0.3f, 0.3f, 0.3f));
	//���f����`�� �֊s��0.0f�œ���
	_modelmgr->Draw(modelhandle, 0.0f);
}

//���[�v�̈ړ�����(����ver)
//SetRopeRad()�̒l�ɂ���Ċp�x���ς��܂�
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
		if (*itr != ROPE_LENGTH_MAX-1)//�͈̓`�F�b�N
		{
			_vec.x = cos(SetRopeRad()) * SetVec().x;
			_vec.y = sin(SetRopeRad()) * SetVec().y;

			_rope[*itr + 1].x = _rope[*itr].x + _vec.x;
			_rope[*itr + 1].y = _rope[*itr].y + _vec.y;
		}
	}
}

//�v���C���[�̌��������Ă��낢��ݒ�
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

//�Q�[���p�b�h���h�����Ă��邩�ǂ���������flag
void Rope::CheckPad(void)
{
	padFlag = _inputinfo.num ? 1 : 0;
}

//���[�v�𔭎˂��鏀��
//�L�[�������ꂽ��ړ������Ɉڂ�
void Rope::Ready(Input* input)
{
	if (_state == ST_ROPE_READY)
	{
		CheckPad();

		for (itr = ropeinfo.begin(); itr != last; itr++)
		{
			_rope[*itr] = _player->ReturnWoToScPos2ver();
		}

		//���ˏ����̃{�^�����ς���Ă��܂� �Eshift�L�[ or pad X
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
		//DrawString(200, 200, "�ҋ@��", 0xfffffff);
#endif
	}
	else {
	}
}

//���[�v�̈ړ��̕��������߂鏈��
//���̏�Ԃ̂Ƃ��́Athe world���
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


		//���\��
		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			 _player->GetPos().y - _tmpOffset.y-5 + dirdraw_Y, 1.0, dirFlag ? AngleRad(45.0f) : AngleRad(135.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, false);

		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			_player->GetPos().y - _tmpOffset.y - 20+  dirdraw_Y, 1.0, AngleRad(0.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, !dirFlag);

		DrawRotaGraph(_player->GetPos().x - _tmpOffset.x + dirNum + dirdraw_X,
			_player->GetPos().y - _tmpOffset.y - 35 + dirdraw_Y, 1.0, dirFlag ? AngleRad(-45.0f) : AngleRad(-135.0f),
			im.ImageIdReturn("image/UI/dirSmall.png", SCENE_RESULT), true, false);



		//���[�v�ҋ@���� Ready�̏�Ԃɖ߂�
		if (_key.keybit.L_LEFT_BUTTON && !_lastkey.keybit.L_LEFT_BUTTON && !padFlag ||
			_key.keybit.X_BUTTON && !_lastkey.keybit.X_BUTTON && padFlag)
		{
			_vec = { 0,0 };
			_state = ST_ROPE_READY;
			_states = &Rope::Ready;
		}

		//�������{�^�� or ���ꂽ�p�x�ɂ���Č��������߂�
		//���������̂��߂Ɋ֐��|�C���^�̈�����input��ݒ肵�Ă���(�{)
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
	//DrawString(200, 200, "�I��", 0xfffffff);
#endif
}

//���[�v�̍s���̏���
//ROPE_LENGTH_MAX�܂ŐL�ё�����
//�ő�܂ōs���Ƒ҂��̏����Ɉڂ�
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

				//�L�΂��Ă���Œ��ɃM�~�b�N��X�e�[�W�ɂ�����΋����I�ɖ߂�
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
		//DrawString(200, 250, "�ړ���", 0xfffffff);
#endif
	}
	else {
	}
}

//time��0�ɂȂ�܂őҋ@�̏���
//������flag��񂷏��Ԃ�ς��Ă܂�
void Rope::Extended(Input* input)
{
	if (_state == ST_ROPE_EXTENDED)
	{
		timerWait--;
		//�����������ǂ����ŃA�j���[�V������i�߂邩�߂�
		ModelManager();

		if (timerWait < 0)
		{
			itr = last;
			_state = ST_ROPE_SHRINKING;
			RopeTurnFlag = true;
			_states = &Rope::Shrinking;
		}

#ifdef _DEBUG
		//DrawString(200, 300, "����", 0xfffffff);
#endif
	}
	else {
	}
}

//���[�v���v���C���[��pos�ɋA�鏈��
//�Ō�܂ŕς���΂܂��ҋ@��ԂɈڂ�
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
		//DrawString(200, 350, "�A�", 0xfffffff);
#endif
	}
	else {
	}
}

//�p�b�h�̃X�e�B�b�N�̊p�x�ɂ���ă��[�v�̌��������߂�
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

//�������{�^���ɂ���Ċp�x��ς��Ă��܂�(����)
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

//�������{�^���ɂ���Ċp�x��ς��Ă��܂�(�Ȑ�)
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

//ROPE_DIR�̏�Ԃɂ����rad�̒l�����߂܂�()
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

//3D�p��̊p�x�Ƃ���ݒ肵�Ă܂�
void Rope::SetRopeRadForDrawZ(void)
{
	switch (_ropeDir)
	{
	case ROPE_DIR_UPPER:
		RopeAngle_Z = dirFlag ? AngleRad(ROPE_THETA) : AngleRad(-ROPE_THETA);
		RopeHitModelNumY = _player->GetStateCrouch() ? 10 :dirFlag ? 35:25;//�����蔻��p
		mentenanceNum_Y = _player->GetStateCrouch() ? -28 + _RopeRect.h / 2 :-58 + _RopeRect.h / 2;//���f���`��
		mentenanceNum_X = dirFlag ? -20 : 20;
		break;
	case ROPE_DIR_LOWER:
		RopeAngle_Z =  dirFlag ? AngleRad(-ROPE_THETA) : AngleRad(ROPE_THETA);
		RopeHitModelNumY = _player->GetStateCrouch() ? 45:70;//�����蔻��p
		mentenanceNum_Y = _player->GetStateCrouch() ? -28 + _RopeRect.h / 2 : -58 + _RopeRect.h / 2;//���f���`��
		mentenanceNum_X = dirFlag ? -20 : 20;
		break;
	case ROPE_DIR_NON:
	case ROPE_DIR_STRAIGHT:
		RopeAngle_Z = AngleRad(0.f);
		RopeHitModelNumY = _player->GetStateCrouch() ?20:55;//�����蔻��p
		mentenanceNum_Y = _player->GetStateCrouch() ? -30 + _RopeRect.h / 2: -60 +_RopeRect.h / 2;//���f���`��
		mentenanceNum_X = dirFlag ? -10 : 10;
		break;
	default:
		break;
	}
}

//�X�s�[�hset�p(������ƎG)
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

//���[�v�̐�[��state��Ԃ��܂�
ROPE_STATE Rope::GetRopeState(void)
{
	//ST_ROPE_READY ���[�v��L�΂�������state �����Ńv���C���[�͓�����ok
	//ST_ROPE_EXTENDING ���[�v��L�΂��Ă���Ԃ�state
	//ST_ROPE_EXTENDED ���[�v���ő�܂ŐL�т���������state
	//ST_ROPE_SHRINKING ���[�v���k�߂Ă���Ԃ�state

	return _state;
}

//���[�v�̑��x��Ԃ��܂�
Position2 Rope::GetRopeVec(void)
{
	return _vec;
}

//�M�~�b�N�ȂǂƓ����������̃��[�v��pos
Position2  Rope::GetReturnPos(void)
{
	return _HitPos;
}

////���[�v�̐�[�̋�`��Ԃ��܂�(Rect)
Rect& Rope::GetRect(void)
{
	return _RopeRect;
}

//���[�v�̐�[�̋�`��Ԃ��܂�(Circle)
Circle& Rope::GetCircle(void)
{
	return _RopeCircle;
}

Circle& Rope::GetCircle2(void)
{
	return _RopeCircle2;
}

//�X�V���ꂽHitClass���󂯎�邽�߂̊֐��ł�
void Rope::GetClass(HitClass* h)
{
	_hit = h;
}