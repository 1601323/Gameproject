#pragma once
#include "Geometry.h"
#include "Object.h"
class Input;

enum GimmickState {
	GM_NONE,	//�ʏ��ԁi�쓮�O�j
	GM_HIT,		//�쓮�O�i���[�v�����������j
	GM_MOVE,	//�쓮��
	GM_MOVE2,	//���ɖ߂��Ă�������ȂǂɁi�h�A���܂�Ƃ��j�i�l�[�~���O�Z���X()�j
	GM_PAUSE,	//�쓮�I���i�J��Ԃ����́j
	GM_END,		//�쓮���S�I��
};

class HitClass;
class Player;

class Gimmick : public Object
{
private:
	HitClass* _hit;
public:
	Gimmick();
	~Gimmick();
	Rect _gmRect;
	CLEAR_DATA _clearData;
	GIMMICK_TYPE _gimType;
	virtual Rect& GetRect() =0;
	GimmickState _state;
	virtual GIMMICK_TYPE& GetType();
	virtual CLEAR_DATA& GetClearData();

	void NextState();
	GimmickState nowState();
	virtual void Updata(Input& input);
	virtual void Updata();
	virtual void Draw(Position2 offset);
};

