#include <DxLib.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "SensorDoor.h"
#include "Geometry.h"
#include "HitClass.h"
#include "Player.h"
#include "Gimmick.h"
#include "ModelMgr.h"
#define HEIGHT 100	//�h�A�̏c���i�Ƃ肠�����Œ�j
#define WIDTH 60	//�h�A�̉����i�Ƃ肠�ȉ����j

SensorDoor::SensorDoor(Position2 pos, Player& p):_pl(p)
{
	//_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();
	openFlag = false;
	//pos�̃I�t�Z�b�g
	_pos.x = pos.x +(MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y +MAP_CHIP_SIZE_Y;

	_gmRect.w = WIDTH ;
	_gmRect.h = HEIGHT;
	count = 0;
	doorCount = 0;
	_state = GM_NONE;
	enterFlag = false;
	_gimType = GIM_DOOR;
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/�o����/door.pmx", SCENE_RESULT);
	//AttachIndex = MV1AttachAnim(modelhandle, 1, -1, false);
	//totalTime = MV1GetAttachAnimTotalTime(modelhandle, AttachIndex);
}

SensorDoor::~SensorDoor()
{
	delete _hit;
	_modelmgr->ModelIdAllDelete();
}
void SensorDoor::Updata() 
{
	//_gmRect.SetCenter(_pos.x,_pos.y-(HEIGHT/2));
	CheckDoMove();
	CheckHit();
	//Draw();
}

void SensorDoor::CheckDoMove()
{
	//���Ԃ����莟��A�v���C���[���h�A�̑O�Ł��������ƊJ���ă��U���g��ʂɔ�΂������������܂��B�����Ă���Ă�������
	//���ꂾ������B�N���A���������������Ă��Ȃ����ǃS�[������H�݂����ȕ����o���Ă�������������Ȃ�
	if (_pl.GetcharState() == ST_VANISH|| _pl.GetcharState() == ST_FEVER) {	//������ڲ԰���X�e���X�A�������̓t�B�[�o�[��Ԃ�������
		_state = GM_MOVE2;
	}
	else {
		_state = GM_MOVE;
	}
	if (_state == GM_NONE) {		//�ŏ��̏�Ԃł���Ή������Ȃ�

	}
	else if (_state == GM_MOVE) {	//�h�A��߂�
		doorCount++;
		if (doorCount % 2 == 0) {
			if (count > 0) {
				count--;
			}
		}
	}
	else if (_state == GM_MOVE2) {	//�h�A���J����
		doorCount++;
		if (doorCount % 2 == 0) {
			if (count < WIDTH / 2) {
				count++;
			}
		}
	}
	else {							//����Ɉ���������Ȃ��������p
	}
}

void SensorDoor::CheckHit()	//�����蔻��̏ꏊ�ɂ���
{
	//if (_pl.GetPos().x+(_pl.GetRect().w /2) <= _pos.x) {		//�v���C���[���h�A��荶���ɂ����画��͉E�h�A�ɂ���
	//	_gmRect.SetCenter(_pos.x +(WIDTH/4)+(count/2),_pos.y-(HEIGHT/2));
	//}
	//else if (_pl.GetPos().x + (_pl.GetRect().w / 2) >= _pos.x) {	//��ڲ԰���h�A���E���ɂ����画��͍��̃h�A�ɂ���
	//	_gmRect.SetCenter(_pos.x-(WIDTH/4)-(count/2),_pos.y-(HEIGHT/2));
	//}
	//else {
	//}
	//�h�A�̂Ƃ���ŏ�������Ƃ͂����悤�ɂ����ق��������炵��
	//if (count * 2 >= _pl.GetRect().w) {
	//�d�l�ύX�Ɋ�Â��āA���ł���������ƃS�[���ł���悤�ɂȂ��Ă��܂�
	//�ȒP��UI�Ő����������ق���������������Ȃ�
		_clearData.midFlag = true;
//#ifdef _DEBUG
//		DrawString(100,30,"�N���A�ł����",GetColor(244,244,244));
//#endif
//	}
//	else {
//		_clearData.midFlag = false;
//#ifdef _DEBUG
//		DrawString(100, 30, "�N���A�ł��Ȃ���", GetColor(244, 244, 244));
//#endif
//
//	}
}

void SensorDoor::Draw(Position2 offset)
{
	//���f���̉�]�p�x�̐ݒ�(���W�A��)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, AngleRad(87.f), 0.0f));
	//���f����pos��ݒ�+���[���h���W����X�N���[���֕ϊ�
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x , _pos.y - offset.y,0.0f)));
	//���f���̊g��k���l�̐ݒ�
	MV1SetScale(modelhandle, VGet(1.f, 1.4f, 1.f));
	//�A�j���[�V�������A�^�b�`(�Ȃ��ł�)
	MV1SetAttachAnimTime(modelhandle, AttachIndex, doorCount);
	
	//���f����֊s��0.0f�ŕ`�� 
	_modelmgr->Draw(modelhandle, 0.0f);

	//�O�g�̕\��
	//DxLib::DrawBox((int)(_pos.x -offset.x-(WIDTH/2)),(int)(_pos.y - offset.y-HEIGHT),(int)(_pos.x - offset.x + (WIDTH/2)),(int)_pos.y - offset.y,0x000000,true);
	//�E��
	//DxLib::DrawBox((int)(_pos.x - offset.x+count),(int)(_pos.y - offset.y - HEIGHT),(int)(_pos.x - offset.x + (WIDTH / 2)),(int)_pos.y - offset.y,0xffffffff,true);
	//����
	//DxLib::DrawBox((int)(_pos.x - offset.x - (WIDTH / 2)),(int)( _pos.y - offset.y - HEIGHT), (int)(_pos.x - offset.x - count ), (int)_pos.y - offset.y, 0xffffffff, true);

	_gmRect.SetCenter(_pos.x , _pos.y - (HEIGHT / 2));
#ifdef _DEBUG
	_gmRect.Draw(offset);
#endif
}

Rect& SensorDoor::GetRect() 
{
	return _gmRect;
}

GIMMICK_TYPE& SensorDoor::GetType()
{
	return _gimType;
}
RESULT_DATA& SensorDoor::GetClearData()
{
	return _clearData;
}