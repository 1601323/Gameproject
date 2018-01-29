#include <DxLib.h>

#include "Geometry.h"
#include "GimButton.h"
#include "HitClass.h"
#include "Player.h"

GimButton::GimButton(Position2 pos, Player& p) :_pl(p)
{
	_hit = new HitClass();
	_state = GM_NONE;
	//�����蔻��̑傫��
	_gmRect.w = 20;
	_gmRect.h = 10;
	//�}�b�v����󂯎�������W�̃I�t�Z�b�g
	_pos.x = pos.x +(MAP_CHIP_SIZE_X/2) - (_gmRect.w /2);
	_pos.y = pos.y + MAP_CHIP_SIZE_Y    -  _gmRect.h;

	count = 180;
	color = 255;

	_gimType = GIM_BUTTON;
}


GimButton::~GimButton()
{
	delete _hit;
	DeleteFontToHandle(Font);
}

void GimButton::Updata() 
{
	CheckDoMove();
	//Draw();
	//_state��none��move�̎����������蔻���\������
	if (_state == GM_NONE || _state == GM_MOVE) {
		_gmRect.SetCenter(_pos.x + (_gmRect.w / 2), _pos.y + (_gmRect.h / 2));	}
	if (_state == GM_HIT) {
		Move();

	}
#ifdef _DEBUG
	if (drawCount <= 120){
		drawCount++;
	}
#endif
}

void  GimButton::CheckDoMove()
{
	if (_state != GM_END && _state != GM_HIT) {
		if (_pl.GetcharState() == ST_VANISH || _pl.GetcharState() ==ST_FEVER) {
			_state = GM_MOVE;
		}
		else  {
			_state = GM_NONE;
		}
		//�v���C���[����ɏ���Ă�����
		if (_state == GM_MOVE && _hit->IsHit(GetRect(), _pl.GetRect())) {
			//_state��GM_HIT�ɕς���
			_state = GM_HIT;
		}
	}
}
//�{�^�����������炨���邱�Ƃɂ���
void GimButton::Move() 
{	
#ifdef _DEBUG
	//DrawString(_pos.x + 20, 60, "�^�w�C�w�C�_", 0xffffff);
#endif
	color -= count;
	if (count <= 0)
	{
		_state = GM_END;
	}
}
void GimButton::Draw(Position2 offset) 
{
	if (_state == GM_NONE){			//��Ԃ�NONE�̎��͉����`�悵�Ȃ��i�b��j

	}
	else if (_state == GM_MOVE) {	//�\�����
		DrawBox(_pos.x-offset.x, _pos.y -offset.y, _pos.x-offset.x + _gmRect.w, _pos.y-offset.y +_gmRect.h, GetColor(0, 200, 100), true);
	}
	else {							//���݂�END��Ԃ̎������ɑJ�ڂ��܂�
		if (count >= 0) {
			count--;
	#ifdef _DEBUG
			//DrawString(340, 400, "�_�j���[�^", 0xffffff);
	#endif
		}
		DrawBox(_pos.x - offset.x,_pos.y - offset.y +(_gmRect.h/2),_pos.x - offset.x +_gmRect.w,_pos.y - offset.y +_gmRect.h,0xff0000,true);
	}
#ifdef _DEBUG
	DrawCircle(_pos.x - offset.x + 70,50 - offset.y,5,GetColor(color,color,color),true);
#endif
	//_state��none��move�̎����������蔻���\������
	if (_state == GM_NONE || _state == GM_MOVE) {
		_gmRect.SetCenter(_pos.x + (_gmRect.w / 2), _pos.y  + (_gmRect.h / 2));
	}
#ifdef _DEBUG
	//_gmRect.Draw(offset);
#endif
}
//�����蔻��p
Rect& GimButton::GetRect() 
{
	return _gmRect;
}

GIMMICK_TYPE& GimButton:: GetType()
{
	return _gimType;
}