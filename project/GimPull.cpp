#include <DxLib.h>
#include "Geometry.h"
#include "GimPull.h"
#include "Rope.h"
#include "HitClass.h"
#include "MapCtl.h"


GimPull::GimPull(Position2 pos,Rope& r):_rope(r),_pos(pos)
{
	_hit = new HitClass();
	_map = MapCtl::GetInstance();
	_state = GM_NONE;
	_gmRect.w = 32*3;
	_gmRect.h = 32;
	count = 60;
	_gimType = GIM_ATTRACT;
}


GimPull::~GimPull()
{	
	delete _hit;
}

void GimPull::Updata(Input& _input) 
{
	//_gmRect.SetCenter(_pos.x+(_gmRect.w/2),_pos.y+(_gmRect.h/2));
	_key = _input.GetInput(1).key;
	_lastKey = _input.GetLastKey();
	_inpInfo = _input.GetInput(1);
	CheckDoMove();
	if (_state == GM_HIT||_state == GM_MOVE) {
		Move();
	}
	//Draw();
}

void GimPull::CheckDoMove() 
{
	if (_state != GM_END) {
		////���[�v�Ƃ̂����蔻������
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			//�������Ă�����
			_state = GM_HIT;
			if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_rope.GetCircle().pos.x > _pos.x) {	//۰�߂̏ꏊ�����S���E���ł����
				_state = GM_MOVE;
			}
		}
		//�f�o�b�N�p�@�����ō쓮�i�ړ��ʂ�ݒ肵�Ȃ��Ɠ������������������܂��j
		//if (_inpInfo.key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
		//	_state = GM_HIT;
		//}
	}
}

void GimPull::Move()
{
	//۰�߂̈ړ��ʂ����Ă���ɏ]���ړ�����\��
	count -=_rope.GetRopeVec().x;
	if (count > 0) {
		if (_state == GM_HIT) {			//������������۰�߂̓��������ꏊ�����S��荶��
			_pos.x-= _rope.GetRopeVec().x;
		}
		else if (_state == GM_MOVE) {	//rope�̓��������ꏊ�����S���E��
			_pos.x+= _rope.GetRopeVec().x;
		}
		else {

		}
	}
	else {		//�ݒ肵���ړ��ʂ����ړ����I��������Ԃ�END�ɕς���
		_state = GM_END;
	}
}

void GimPull::Draw(Position2 offset) 
{
	if (_state != GM_END) {			//END�ȊO�ł���ΐF�͓����܂�
		DrawBox((int)_pos.x - offset.x,(int) _pos.y-offset.y,(int) _pos.x -offset.x+ 32 * 3, (int)_pos.y - offset.y + 32, GetColor(0, 216, 140), true);
	}
	else if (_state == GM_END) {	//END�ɂȂ�����Ԃɕς���
		DrawBox((int)_pos.x - offset.x, (int)_pos.y - offset.y,(int) _pos.x - offset.x + 32 * 3, (int)_pos.y - offset.y + 32, GetColor(255, 0, 0), true);
	}
	else{}
	_gmRect.SetCenter(_pos.x -offset.x+ (_gmRect.w / 2), _pos.y -offset.y + (_gmRect.h / 2));
	_gmRect.Draw();
}
//�������`��Ԃ�
Rect& GimPull::GetRect() 
{
	return _gmRect;
}

GIMMICK_TYPE& GimPull::GetType()
{
	return _gimType;
}