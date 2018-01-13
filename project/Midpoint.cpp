#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Player.h"
#include "HitClass.h"
#include "GameMain.h"
#include "Midpoint.h"



Midpoint::Midpoint()
{
	_hit = new HitClass();
	GetFlag = false;
	//�}�b�v����󂯎��悤�ɂ���
	_pos.x = 400;
	_pos.y = 100;
	tmpDir = DIR_RIGHT;
	//�傫���ɂ��Ă͂Ƃ肠�����`�b�v�̑傫���ƈꏏ�ɂ��Ă���
	_hitRect.w = 32;
	_hitRect.h = 32;
	cnt = 0;
	bubbleFlag = false;
	bubble = 0;
}
void Midpoint::GetClass(Player* p)
{
	_pl = p;
}
Midpoint::~Midpoint()
{
	delete _hit;
}

void Midpoint::Updata()
{
	GetPoint();
	if (GetFlag == true) {
		FollowDir();
	}
}
void Midpoint::GetPoint()
{
	//��ڲ԰�����ǂ蒅�����Ƃ��i�����蔻��?�j
	if (_hit->IsHit(GetRect(), _pl->GetRect())) {
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
	_pos.x = _pl->GetPos().x;
	_pos.y = _pl->GetPos().y;
	if (tmpDir == DIR_RIGHT) {
		_pos.x = _pl->GetPos().x -30;
		_pos.y = _pl->GetPos().y - bubble;
	}
	else if (tmpDir == DIR_LEFT) {
		_pos.x = _pl->GetPos().x + 30;
		_pos.y = _pl->GetPos().y -bubble;
	}

}
void Midpoint::Draw(Position2 offset)
{
	if (GetFlag == false) {
		DrawCircle(_pos.x - offset.x + (_hitRect.w / 2), _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(210, 140, 44), true);
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
		_hitRect.Draw(offset);
	}
	else if (GetFlag == true) {
		DrawCircle(_pos.x - offset.x + (_hitRect.w / 2) , _pos.y - offset.y + (_hitRect.h / 2), 12, GetColor(0, 240, 44), true);
		//�悭�l�����瓖���蔻��擾�����ア��Ȃ����
		_hitRect.SetCenter(_pos.x + (_hitRect.w / 2), _pos.y + (_hitRect.h / 2));
		_hitRect.Draw(offset);
	}

}
bool Midpoint::ReturnGetFlag()
{
	return GetFlag;
}

Rect& Midpoint::GetRect()
{
	return _hitRect;
}