#pragma once
#include "Enemy.h"

#define EM_LOOKBACK_TIME 240
#define EM_LOOKBACK_MODEL_TIME 3

class HitClass;
class EnemyServer;
class Player;
class MapCtl;
class Rope;
class ModelMgr;

class EmLookback :
	public Enemy
{
private:
	HitClass& _hit;
	EnemyServer& _server;
	DIR _dir;			//��ׂ̌���
	MapCtl* _map;
	ModelMgr* _modelmgr;

	Player& _player;
	Rope& _rope;
	int LookCount;		//�U��Ԃ�̶���
	int FearCount;		//���̶݂���
	int LookModelDirCnt;//���f�����U��Ԃ�J�E���g

	//void Draw();
	int circle;
	int loseSightCnt;	//PL���F���͈͂���O��Č������܂ł̃J�E���g

	int AnimeIndex;     //�A�j���[�V�����p�C���f�b�N�X
	int AnimTotalTime;  //�A�j���[�V��������
	float AnimNowTime;  //���݂̃A�j���[�V��������
	int AnimWheelTimer; //�^�C����]�p�̃^�C�}�[

	int modelhandle;
	int textureIndex;
	int textureIndexWheel;            //�^�C���p�̃e�N�X�`��
	float modelDirAngle;
	int eyeRange;
	ENEMY_STATE _state;
	ENEMY_DATA _emData;
	Circle _emEye;
	Position2 _pos;
	Position2 _tmpOffset;
	Position2 _initPos;
	EnemyServerData _individualData;	//�G�l�~�[�{���ɑ���f�[�^
	ENEMY_RANGE_LEVEL _rangeLevel;

	float emSpeed; 
	float upAngle;
	float downAngle;
	bool returnFlag;
	bool ModelDirChangeFlag;            //���f�����U��Ԃ�A�j���[�V����flag
	float vx;
	float vy;
	bool midFlag;
	void SetMove();						//�����ɂ��ăZ�b�g����
	void setDir(void);					//����
	void Visibility();					//���E����
	void LookPl(void);					//�v���C���[������
	void LoseSight();
	void moveFear(void);				//���ݏ��
	void EnemyFalter();					//���ݏ�ԂɑJ�ڂ���
	void ReturnPoint();					//���̈ʒu�ɖ߂��Ă���
	void returnDir(Position2 offset);	//�����Ă��������Ԃ�
	void Gravity();
	void SetRange();
	void LimitMove();					//�����ɐ���������
	void TurnPlayer();					//��ڲ԰���������Ă�������������
public:
	EmLookback(Position2 pos,Player& pl,Rope& rope,EnemyServer& server,HitClass&  hit);
	~EmLookback();
	Rect _emRect;
	Rect& GetRect();
	DIR GetDir();
	ENEMY_TYPE _emType;
	ENEMY_TYPE& GetType();
	void SetInitPos();
	void Updata();

	ENEMY_STATE & GetState();

	void GetClass(HitClass* hit, Player& pl);

	void Draw(Position2 offset);

};

