#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Player.h"
#include "HitClass.h"
#include "GameMain.h"
#include "Midpoint.h"
#include "ModelMgr.h"


Midpoint::Midpoint()
{
	_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();
	GetFlag = false;
	//�}�b�v����󂯎��悤�ɂ���
	_pos.x = 350;
	_pos.y = 150;
	//mid2�̏ꏊ���󂯎��
	_midPos.x = 600;
	_midPos.y = 480;
	initPos = _pos;
	tmpDir = DIR_RIGHT;
	//�傫���ɂ��Ă͂Ƃ肠�����`�b�v�̑傫���ƈꏏ�ɂ��Ă���
	_hitRect.w = 32;
	_hitRect.h = 32;
	//���ԓ_�̑傫���ɂ���
	_midRect.w = 32;
	_midRect.h = 32;

	cnt = 0;
	bubbleFlag = false;
	bubble = 0;
	modelhandle = _modelmgr->ModelIdReturn("gimmick_model/��/��2-10.pmx", SCENE_RESULT);
}
void Midpoint::GetClass(Player* p)
{
	_pl = p;
}
Midpoint::~Midpoint()
{
	delete _hit;
	_modelmgr->ModelIdAllDelete();
}

void Midpoint::Updata()
{
	GetPoint();
	if (GetFlag == true||checkpointFlag == true) {
		FollowDir();
	}
}
void Midpoint::GetPoint()
{
	//��ڲ԰�����ǂ蒅�����Ƃ��i�����蔻��?�j
	if (_hit->IsHit(GetRect(), _pl->GetRect())) {
		checkpointFlag = true;
	}
	//�ړI���擾����
	if (checkpointFlag == true && _hit->IsHit(GetRect2(), _pl->GetRect())) {
		GetFlag = true;
	}
}
void Midpoint::FollowDir()
{
	cnt++;
	if (cnt > 5) {
		cnt = 0;
		if (bubbleFlag == false) {
			bubble++;
		}
		else if (bubbleFlag == true) {
			bubble--;
		}
	}
	if (bubble > 10) {
		bubbleFlag = true;
	}
	else if (bubble < -1) {
		bubbleFlag = false;
	}
	//�v���C���[�̌��ɂ��Ă�������
	if (_pl->GetDir() == DIR_RIGHT || _pl->GetDir() == DIR_LEFT) {
		tmpDir = _pl->GetDir();
	}
	if (GetFlag == true) {

		_midPos.x = _pl->GetPos().x;
		_midPos.y = _pl->GetPos().y;
		if (tmpDir == DIR_RIGHT) {
			_midPos.x = _pl->GetPos().x - 45;
			_midPos.y = _pl->GetPos().y - bubble;
		}
		else if (tmpDir == DIR_LEFT) {
			_midPos.x = _pl->GetPos().x + 45;
			_midPos.y = _pl->GetPos().y - bubble;
		}
	}
	else if (checkpointFlag == true) {

		_pos.x = _pl->GetPos().x;
		_pos.y = _pl->GetPos().y;
		if (tmpDir == DIR_RIGHT) {
			_pos.x = _pl->GetPos().x - 45;
			_pos.y = _pl->GetPos().y - bubble;
		}
		else if (tmpDir == DIR_LEFT) {
			_pos.x = _pl->GetPos().x + 45;
			_pos.y = _pl->GetPos().y - bubble;
		}
	}
}
void Midpoint::Draw(Position2 offset)
{
	MV1SetPosition(modelhandle, VGet(_pos.x - offset.x + (_hitRect.w / 2),SCREEN_SIZE_Y - _pos.y + offset.y - (_hitRect.h), 0));
	MV1SetScale(modelhandle, VGet(0.2f, 0.2f, 0.2f));
	MV1DrawModel(modelhandle);
	_modelmgr->SetMaterialDotLine(modelhandle, 0.2f);

	if (checkpointFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2), _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(210, 140, 44), true);
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}
	else if (checkpointFlag == true && GetFlag == false) {
		//DrawCircle(_pos.x - offset.x + (_hitRect.w / 2) , _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(0, 240, 44), true);
		//�悭�l�����瓖���蔻��擾�����ア��Ȃ����
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
#ifdef _DEBUG
		_hitRect.Draw(offset);
#endif
	}
	if (GetFlag == false) {
		DrawBox(_midPos.x - offset.x, _midPos.y - offset.y, _midPos.x + _midRect.w - offset.x, _midPos.y + _midRect.h - offset.y, 0xcccccc, true);
		_midRect.SetCenter(_midPos.x+(_midRect.w/2),_midPos.y+(_midRect.h /2));
	}
	else if(GetFlag == true ){
		DrawCircle(_midPos.x - offset.x + (_midRect.w / 2) , _midPos.y - offset.y + (_midRect.h / 2), 12, GetColor(0, 240, 44), true);
	}
	_midRect.Draw(offset);
}
bool Midpoint::ReturnGetFlag()
{
	return GetFlag;
}
bool Midpoint::ReturnCheckFlag()
{
	return checkpointFlag;
}
Rect& Midpoint::GetRect()
{
	return _hitRect;
}
Rect& Midpoint::GetRect2()
{
	return _midRect;
}
Position2& Midpoint:: GetInitPos()
{
	return initPos;
}