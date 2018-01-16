#pragma once
#include "Gimmick.h"
#include "Geometry.h"
#include "Input.h"
class Input;
class Rope;
class MapCtl;
class ModelMgr;

class GimPull :
	public Gimmick
{
private:
	HitClass* _hit;
	Rope& _rope;
	GimmickState _state;
	Position2 _pos;
	MapCtl* _map;
	ModelMgr* _modelmgr;
	//�m�F�p
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	int count;				//���ʓ����͈͂�ݒ肷��
	int modelhandle;

	void CheckDoMove();		//�����������
	void Move();			//�M�~�b�N�̓���ɂ���
public:
	GimPull(Position2 pos,Rope& r);
	~GimPull();
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	void Updata(Input& _input);
	void Draw(Position2 offset);			//�`��

};

