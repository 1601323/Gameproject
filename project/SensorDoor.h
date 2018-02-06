#pragma once
#include <memory>
#include <list>
#include <map>
#include "Gimmick.h"
#include "Geometry.h"

class HitClass;
class Player;
class ModelMgr;

class SensorDoor :
	public Gimmick
{
private:
	HitClass* _hit;
	Player& _pl;
	ModelMgr* _modelmgr;
	GimmickState  _state;
	Position2 _pos;
	//����m�F�p
	bool keyFlag;
	bool openFlag;

	int count;			//�h�A�J�̂��߂̃J�E���g
	int doorCountOpen;		//�h�A�J�̒����J�E���g
	int doorCountClose;		//�h�A�J�̒����J�E���g


	int modelhandle;
	int AttachIndex;
	int totalTime;

	int AttachIndexClose;
	int totalTimeClose;

	void CheckDoMove();	//�����������
	void CheckHit();	//�����蔻����v���C���[�̏ꏊ�ɂ���ĕς���̂ŏ����Ă���
	void Draw(Position2 offset);		//�`��
	void DrawStartDoor();               //�X�e�[�W�J�n���ɌĂ�
public:
	SensorDoor(Position2 pos,Player& p);
	~SensorDoor();
	bool enterFlag;
	void Updata();
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	RESULT_DATA& GetClearData();
};

