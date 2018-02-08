#pragma once
#include "Gimmick.h"
#include "Geometry.h"
#include "Input.h"
class Input;
class Rope;
//class Player;
class MapCtl;
class ModelMgr;

class GimPull :
	public Gimmick
{
private:
	HitClass* _hit;
	Rope& _rope;
	Player& _player;
	GimmickState _state;
	Position2 _pos;
	MapCtl* _map;
	ModelMgr* _modelmgr;
	Position2 _initPos;
	//�m�F�p
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	int count;				//���ʓ����͈͂�ݒ肷��
	int modelhandle;
	int textureIndex;       //�e�N�X�`����index

	void CheckDoMove();		//�����������
	void Move();			//�M�~�b�N�̓���ɂ���
	void SetInit();
public:
	GimPull(Position2 pos,Rope& r,Player& p);
	~GimPull();
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	void Updata(Input& _input);
	void Draw(Position2 offset);			//�`��
	void ChangeStageTexture();
};

