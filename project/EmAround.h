#pragma once
#include "Enemy.h"

#define GRAVITY 0.9f
class MapCtl;
class HitClass;
class Player;
class Rope;
class EnemyServer;

class EmAround :
	public Enemy
{
private:
	HitClass* _hit;
	Player& _pl;
	Rope& _rope;
	EnemyServer& _server;
	Position2 _pos;
	Rect _emRect;
	Circle _emEye;
	ENEMY_STATE _state;
	ENEMY_DATA _emData;
	MapCtl* _map;
	DIR _dir;
	EnemyServerData _individualData;		//��а�{���ɑ����ް�

	//�d�͂ɂ���
	float vx;
	float vy;
	float speed;			
	bool moveFlag;			//������؂�ւ��邽�߂̃t���O
	int dis;				//�ǂꂭ�炢�������甽�΂������̂���`����
	float interCnt;			//���Α��������܂ł̃J�E���g

	int loseSightCnt;		//PL���F���͈͂���O��Č������܂ł̃J�E���g
	int fearCnt;			//�Ђ��ł���b��

	void Move();			//�G�̓����ɂ��Ă̊Ǘ����s��

	void BasicMove();		//��������Ԃ̊�{����
	void InterMove();		//��������Ԃ̓r������
	void FoundMove();		//��ڲ԰�������̓���
	void CheckMove();		//�}�b�v�Ƃ̂����蔻��ɂ���
	void Visibility();		//�G�̎��E�Ɋւ���
	void moveFear();		//���ݏ�ԂɊւ���
	void LoseSight();		//��ڲ԰�����������Ƃ�
	void Gravity();			//�d�͔���
public:
	EmAround(Position2 pos,Player& pl,Rope& rope,EnemyServer& server);
	~EmAround();

	void Updata();	
	Rect& GetRect();
	ENEMY_STATE & GetState();
	void EnemyFalter();		//���ݏ�ԂɂȂ����
	void GetClass(HitClass* hit,Player& pl);		//�N���X�󂯎��p

	void Draw(Position2 offset);
};

