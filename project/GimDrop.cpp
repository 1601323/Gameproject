#include <DxLib.h>
#include "GimDrop.h"
#include "HitClass.h"
#include "Input.h"
#include "Rope.h"
#include "Player.h"
#include "MapCtl.h"



GimDrop::GimDrop(Position2 pos,Rope& r,Player& p):_rope(r),_player(p)
{
	_hit = new HitClass();
	_map = MapCtl::GetInstance();
	_state = GM_NONE;
	gravity = 0.9;
	velocity = 0;
	_gmRect.h = 23;
	_gmRect.w = 23;
	_pos.x = pos.x + (MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y+MAP_CHIP_SIZE_Y - (_gmRect.h / 2);
	dropFlag = false;
	count = 60;

	_gimType = GIM_FALL;
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
	CheckDoMove();
	if (_state == GM_HIT) {
		MoveLeft();
	}
	else if (_state == GM_MOVE) {
		MoveRight();
	}
	else {

	}
	//Draw();	
}
//_state�̕ύX�ɂ���
void GimDrop::CheckDoMove()
{
	//if (_state == GM_NONE ||_state== GM_PAUSE) {	//��Ԃ���x�����؂�ւ��Ȃ��悤�ɂ���
	if(_state != GM_HIT && _state != GM_MOVE){							//�����Ă���r���ł����Ă�������]�����ł���悤�ɂȂ��Ă܂�
		////���[�v�Ƃ̂����蔻������
		if (_rope.GetRopeState()== ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			if (_rope.GetCircle().pos.x < _pos.x) {	//rope��ڸĂ�X�����̂�X���傫��
				_state = GM_HIT;		//�����ɓ���
			}
			else {
				_state = GM_MOVE;		//�E���ɓ���
			}
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
	//�s����̏������邩�ǂ����𒲂ׂ�
	Position2 nextPos = _pos;
	nextPos.x = _pos.x + (_gmRect.h / 2);
	nextPos.y = _pos.y + (_gmRect.w);
	//�����̍ۂ̂��߂ɔ��Α������肵�Ă���
	Position2 nextPos1 = _pos;
	nextPos1.x = _pos.x - (_gmRect.w / 2);
	nextPos1.y = _pos.y + (_gmRect.h);
	//���ʂ̍s����
	//�Ƃ肠�������̍s���悾��
	Position2 flontPos = _pos;
	flontPos.x = _pos.x - (_gmRect.w / 2);
	//rope�̕������܂�������������
	if (_rope._ropeDir == ROPE_DIR_STRAIGHT) {
		//�s����Ƀv���C���[��������~�܂�
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//�ǂ���������~�܂�
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//��{����
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//۰�߂��ǂ��󂯎�邩���߂Ă��Ȃ��̂łƂ肠�����ޯ���߰��Ŏ~�܂�悤�ɂ���
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//_state = GM_END;
			}
			else if (_map->GetChipType(nextPos) == CHIP_BLANK &&_map->GetChipType(nextPos1) == CHIP_BLANK) {	//�s����ɏ����Ȃ���Η�����
				dropFlag = true;
			}
			//۰�߂̈ړ��ʂ��󂯎��
			_pos.x += _rope.GetRopeVec().x;
			//if (_rope._ropeDir != ROPE_DIR_STRAIGHT) {	//�܂������̎��͂������ɂ͈ړ������Ȃ�
			//	_pos.y += _rope.GetRopeVec().y;
			//}
			if (_rope.GetRopeVec().x == 0) {
				_state = GM_PAUSE;
			}
		}
		else {	//�s����ɏ����Ȃ������ꍇ�i�����^���j
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//��������ɒn�ʂ�����Ί���ē�����I������
				_state = GM_END;
			}
			velocity += gravity;
			_pos.y += (int)velocity / 2;
		}
	}
	else {	//����ȊO�̎�
			//�s����Ƀv���C���[��������~�܂�
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//�ǂ���������~�܂�
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//��{����
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//۰�߂��ǂ��󂯎�邩���߂Ă��Ȃ��̂łƂ肠�����ޯ���߰��Ŏ~�܂�悤�ɂ���
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//_state = GM_END;
			}
			//else if (_map->GetChipType(nextPos) == CHIP_BLANK) {	//�s����ɏ����Ȃ���Η�����
			//	dropFlag = true;
			//}
			//۰�߂̈ړ��ʂ��󂯎��
			_pos.x += _rope.GetRopeVec().x;
			_pos.y += _rope.GetRopeVec().y;
	
			if (_rope.GetRopeVec().x == 0) {
				_state = GM_PAUSE;
			}
		}
		//else {	//�s����ɏ����Ȃ������ꍇ�i�����^���j
		//	if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//��������ɒn�ʂ�����Ί���ē�����I������
		//		_state = GM_END;
		//	}
		//	velocity += gravity;
		//	_pos.y += (int)velocity / 2;
		//}
	}

}

void GimDrop::MoveRight()
{
	//�s����̏������邩�ǂ����𒲂ׂ�
	Position2 nextPos = _pos;
	nextPos.x = _pos.x - (_gmRect.w / 2);
	nextPos.y = _pos.y + (_gmRect.h);
	//���Α������肵�Ă���
	Position2 nextPos1 = _pos;
	nextPos1.x = _pos.x + (_gmRect.h / 2);
	nextPos1.y = _pos.y + (_gmRect.w);
	//���ʂ̍s����
	//�Ƃ肠�����E�̍s���悾��
	Position2 flontPos = _pos;
	flontPos.x = _pos.x + (_gmRect.w / 2);
	if (_rope._ropeDir == ROPE_DIR_STRAIGHT) {
		//�s����Ƀv���C���[��������~�܂�
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//�ǂ���������~�܂�
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//��{����	

			if (_map->GetChipType(nextPos) == CHIP_BLANK && _map->GetChipType(nextPos1)== CHIP_BLANK) {	//�s����ɏ����Ȃ���Η�����
				dropFlag = true;
			}
			else if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//۰�߂��ǂ��󂯎�邩���߂Ă��Ȃ��̂łƂ肠�����ޯ���߰��Ŏ~�܂�悤�ɂ���
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//۰�߂̈ړ��ʂ��󂯎��
				_pos.x += _rope.GetRopeVec().x;
				if (_rope.GetRopeVec().x == 0) {
					_state = GM_PAUSE;
				}
			}
		}
		else {	//�s����ɏ����Ȃ������ꍇ�i�����^���j
			nextPos.y = _pos.y + (_gmRect.h * 2);
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//��������ɒn�ʂ�����Ί���ē�����I������
				_state = GM_END;
			}
			velocity += gravity;
			_pos.y += (int)velocity / 2;
		}
	}
	else {
		//�s����Ƀv���C���[��������~�܂�
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//�ǂ���������~�܂�
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//��{����	

			//if (_map->GetChipType(nextPos) == CHIP_BLANK) {	//�s����ɏ����Ȃ���Η�����
			//	dropFlag = true;
			//}
		if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//۰�߂��ǂ��󂯎�邩���߂Ă��Ȃ��̂łƂ肠�����ޯ���߰��Ŏ~�܂�悤�ɂ���
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//۰�߂̈ړ��ʂ��󂯎��
				_pos.x += _rope.GetRopeVec().x;
				_pos.y += _rope.GetRopeVec().y;
	
				if (_rope.GetRopeVec().x == 0) {
					_state = GM_PAUSE;
				}
			}
		}
		//else {	//�s����ɏ����Ȃ������ꍇ�i�����^���j
		//	nextPos.y = _pos.y + (_gmRect.h * 2);
		//	if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//��������ɒn�ʂ�����Ί���ē�����I������
		//		_state = GM_END;
		//	}
		//	velocity += gravity;
		//	_pos.y += (int)velocity / 2;
		//}
	}
	

}

//�`��
void GimDrop::Draw(Position2 offset) 
{
	if (_state != GM_END && _state != GM_PAUSE) {
		//���̂��̂̕`��
		DrawCircle(_pos.x - offset.x,_pos.y -offset.y,10,GetColor(255,0,255),true);
	}
	else if (_state == GM_END) {
		//���ꂽ�肵�Ă�`��
		count--;
		if (count >= 0) {
			DrawString(_pos.x - offset.x -70, _pos.y - offset.y -30, "�_�K�V���[���^", 0xffff00);
		}
		DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(255, 0, 0), true);
	}
	else if (_state == GM_PAUSE) {		//�ړ����ꎞ��~���Ă���Ƃ��i�ǂɂԂ������Ȃ�)
		DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(0, 0, 255), true);
	}
	_gmRect.SetCenter(_pos.x -offset.x, _pos.y - offset.y);
	_gmRect.Draw();
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