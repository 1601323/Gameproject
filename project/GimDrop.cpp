#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "GimDrop.h"
#include "HitClass.h"
#include "Input.h"
#include "Rope.h"
#include "Player.h"
#include "MapCtl.h"
#include "ModelMgr.h"


GimDrop::GimDrop(Position2 pos,Rope& r,Player& p):_rope(r),_player(p)
{
	_hit = new HitClass();
	_map = MapCtl::GetInstance();
	_modelmgr = ModelMgr::Instance();
	_state = GM_NONE;
	gravity = 0.9;
	velocity = 0;
	_gmRect.h = 30;
	_gmRect.w = 30;
	_pos.x = pos.x + (MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y+MAP_CHIP_SIZE_Y - (_gmRect.h / 2);
	dropFlag = false;
	count = 60;
	_fd.feverCnt = 0;
	_fd.feverTime = 0;

	_gimType = GIM_FALL;
	//���f���ǂݍ���
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/�H�p��/�p���ꖇ.pmd", SCENE_RESULT);
}


GimDrop::~GimDrop()
{
	delete _hit;
}

void GimDrop::Updata(Input& _input)
{
	//_gmRect.SetCenter(_pos.x , _pos.y);

	_key = _input.GetInput(1).key;
	_lastKey = _input.GetLastKey();
	_inpInfo = _input.GetInput(1);
	if (_state != GM_END) {
		CheckDoMove();
	}
	if (_state == GM_HIT) {
		MoveLeft();
		_fd.feverCnt = 1;

		SendFeverData();

	}
	else if (_state == GM_MOVE) {
		MoveRight();
		_fd.feverCnt = 1;
		SendFeverData();

	}
	else {

	}
	GetItem();
	_fd =GameMain::Instance().ReturnFeverData();
	//cout << _fd.feverCnt << endl;
	//Draw();	
}
//_state�̕ύX�ɂ���
void GimDrop::CheckDoMove()
{
	//if (_state == GM_NONE ||_state== GM_PAUSE) {	//��Ԃ���x�����؂�ւ��Ȃ��悤�ɂ���
	if(_state != GM_HIT && _state != GM_MOVE){							//�����Ă���r���ł����Ă�������]�����ł���悤�ɂȂ��Ă܂�
		////���[�v�Ƃ̂����蔻������
		if (_rope.GetRopeState()== ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			//if (_rope.GetCircle().pos.x < _pos.x) {	//rope��ڸĂ�X�����̂�X���傫��
				_state = GM_HIT;		//�����ɓ���
			//}
			//else {
			//	_state = GM_MOVE;		//�E���ɓ���
			//}
		}	
		//�f�o�b�N�p�@�����ō쓮�i�ړ��ʂ�ݒ肵�Ȃ��Ɠ������������������܂��j
		//if (_inpInfo.key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
		//	//�����n�߂�
		//	_state = GM_HIT;
		//}
	}
}

void GimDrop::MoveLeft()
{

	//۰�߂̈ړ��ʂ��󂯎��
	_pos.x += _rope.GetRopeVec().x;
	_pos.y -= _rope.GetRopeVec().y;

	if (_rope.GetRopeVec().x == 0) {
		_state = GM_PAUSE;
	}
}

void GimDrop::MoveRight()
{
	//۰�߂̈ړ��ʂ��󂯎��
	_pos.x += _rope.GetRopeVec().x;
	_pos.y -= _rope.GetRopeVec().y;
	if (_rope.GetRopeVec().x == 0) {
		_state = GM_PAUSE;
	}
}
void GimDrop::GetItem()
{
	if (_hit->IsHit(_gmRect, _player.GetRect())) {
		if (_state != GM_END) {
			_state = GM_END;
			_fd.feverCnt++;
			GameMain::Instance().SetFeverData(_fd);
		}
	}
}
//�`��
void GimDrop::Draw(Position2 offset) 
{
	//���f���̉�]�p�x�̐ݒ�(���W�A��)
	MV1SetRotationXYZ(modelhandle, VGet(AngleRad(90.0f), 0.0f, 0.0f));
	//���f����pos��ݒ�+���[���h���W����X�N���[���֕ϊ�
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x, _pos.y - offset.y, 0)));
	//���f���̊g��k���l�̐ݒ�
	MV1SetScale(modelhandle, VGet(15.0f, 15.0f, 15.0f));
	//���f����֊s��0.0f�ŕ`�� 
	if (_state != GM_END && _state != GM_PAUSE) {
		//���̂��̂̕`��
		//DrawCircle(_pos.x - offset.x,_pos.y -offset.y,10,GetColor(255,0,255),true);
		_modelmgr->Draw(modelhandle, 0.0f);
	}
	else if (_state == GM_END) {
		//���ꂽ�肵�Ă�`��
		count--;
		if (count >= 0) {
			//DrawString(_pos.x - offset.x -70, _pos.y - offset.y -30, "�_�K�V���[���^", 0xffff00);
		}
		//_modelmgr->Draw(modelhandle, 0.0f);
		//DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(255, 0, 0), true);
	}
	else if (_state == GM_PAUSE) {		//�ړ����ꎞ��~���Ă���Ƃ��i�ǂɂԂ������Ȃ�)
		//DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(0, 0, 255), true);
		_modelmgr->Draw(modelhandle, 0.0f);
	}
	_gmRect.SetCenter(_pos.x , _pos.y);
#ifdef _DEBUG
	_gmRect.Draw(offset);
#endif
}
//�����蔻��p
Rect& GimDrop::GetRect()
{
	return _gmRect;
}
//gimmick�̎�ނ�Ԃ����߂̊֐�
GIMMICK_TYPE& GimDrop::GetType()
{
	return _gimType;
}

void GimDrop::SendFeverData()
{
	GameMain::Instance().SetFeverData(_fd);
}