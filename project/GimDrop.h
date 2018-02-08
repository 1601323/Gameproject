#pragma once
#include "Gimmick.h"
#include "Geometry.h"
#include "Input.h"
class Input;
class Rope;
class player;
class MapCtl;
class ModelMgr;

class GimDrop :
	public Gimmick
{
private:
	HitClass* _hit;
	Rope& _rope;
	Player& _player;
	MapCtl* _map;
	ModelMgr* _modelmgr;
	GimmickState _state;
	Position2 _pos;
	Position2 _initPos;
	FEVER_DATA _fd;

	bool dropFlag;	//�����t���O
	float gravity;	//�d��
	float velocity;	//���x
	//�m�F�p
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	int count;		//�\���b����ݒ�
	int modelhandle;//���f��
	void CheckDoMove();
	void Draw(Position2 offset);
	//ڸĂƂ̔�������ē������������߂�
	void MoveLeft();	
	void MoveRight();
	//̨��ް���тƂ��ăQ�b�g����
	void GetItem();
	void SetInit();
public:
	GimDrop(Position2 pos,Rope& r,Player& p);
	~GimDrop();
	//Rect _gimRect;
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	void Updata(Input& input);
	void SendFeverData();
	
};

