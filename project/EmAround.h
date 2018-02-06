#pragma once
#include "Enemy.h"

class MapCtl;
class HitClass;
class Player;
class Rope;
class EnemyServer;
class ModelMgr;

class EmAround :
	public Enemy
{
private:
	HitClass& _hit;
	Player& _pl;
	Rope& _rope;
	EnemyServer& _server;
	Position2 _pos;
	Position2 _initPos;
	Rect _emRect;
	Circle _emEye;
	ENEMY_STATE _state;
	ENEMY_DATA _emData;
	MapCtl* _map;
	DIR _dir;
	EnemyServerData _individualData;		//��а�{���ɑ����ް�
	ModelMgr* _modelmgr;
	ENEMY_RANGE_LEVEL _rangeLevel;

	//�d�͂ɂ���
	float vx;
	float vy;
	float speed;			
	bool moveFlag;			//������؂�ւ��邽�߂̃t���O
	bool ModelDirChangeFlag;//���f�����U��Ԃ�A�j���[�V����flag
	int dis;				//�ǂꂭ�炢�������甽�΂������̂���`����
	float interCnt;			//���Α��������܂ł̃J�E���g

	int loseSightCnt;		//PL���F���͈͂���O��Č������܂ł̃J�E���g
	int fearCnt;			//�Ђ��ł���b��

	bool midFlag;			//���ԓ_�t���O

	int AnimeIndex;     //�A�j���[�V�����p�C���f�b�N�X
	int AnimTotalTime;  //�A�j���[�V��������
	float AnimNowTime;    //���݂̃A�j���[�V��������
	int AnimWheelTimer; //�^�C����]�p�̃^�C�}�[


	//�RD�����ɂ���
	int modelhandle;
	int textureIndex;
	int textureIndexWheel;            //�^�C���p�̃e�N�X�`��
	float modelDirAngle;

	void Move();			//�G�̓����ɂ��Ă̊Ǘ����s��

	void BasicMove();		//��������Ԃ̊�{����
	void InterMove();		//��������Ԃ̓r������
	void FoundMove();		//��ڲ԰�������̓���
	void CheckMove();		//�}�b�v�Ƃ̂����蔻��ɂ���
	void Visibility();		//�G�̎��E�Ɋւ���
	void moveFear();		//���ݏ�ԂɊւ���
	void LoseSight();		//��ڲ԰�����������Ƃ�
	void Gravity();			//�d�͔���
	void SetRange();		//���E�̍L�������x���ʂɔ��肵�܂�

	Position2 tmpPos;

public:
	EmAround(Position2 pos,Player& pl,Rope& rope,EnemyServer& server,HitClass& hit);
	~EmAround();

	void Updata();	
	Rect& GetRect();
	DIR GetDir();
	ENEMY_STATE & GetState();
	void EnemyFalter();		//���ݏ�ԂɂȂ����
	void GetClass(HitClass* hit,Player& pl);		//�N���X�󂯎��p
	void SetInitPos();

	void Draw(Position2 offset);
};

