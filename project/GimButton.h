#pragma once
#include "Gimmick.h"
#include "Geometry.h"
class Player;
class HitClass;
class GimButton :
	public Gimmick
{
private:
	HitClass* _hit;
	Player& _pl;
	GimmickState _state;
	Position2 _pos;
	Position2 _initPos;

	//�V�ѐS
	//�����ꂽ��
	int count;
	int drawCount;
	int color;
	int Font;	
	void CheckDoMove();		//�쓮�̂��߂̏���
	void Move();			//�M�~�b�N�̓��e�ɂ���
	void Draw(Position2 offset);			//�`��
	void SetInit();			//���Ȃ����̃��Z�b�g���s��
public:
	GimButton(Position2 pos,Player& p);
	~GimButton();
	Rect _gmRect;
	Rect& GetRect();		//�����蔻��p�̃��N�g��Ԃ�
	GIMMICK_TYPE& GetType();
	void Updata();
};

