#pragma once
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Scene.h"
#include "Geometry.h"
#include "Input.h"

class Gimmick;
class Player;
class Enemy;
class MapCtl;
class Rope;
class HitClass;
class Camera;
class EnemyServer;
class Midpoint;
class TimeManager;

class ModelManager;

class GimmickFactory;
class EnemyFactory;


class GameScene :public Scene
{
private:
	void(GameScene::*_updater)(Input* input);		//�V�[���J�ڗp
	Player* _player;
	Enemy* _enemy;
	Camera* _cam;
	Rope* _rope;	
	HitClass* _hit;
	MapCtl* _map;
	EnemyServer* _server;
	Midpoint* _mid;
	TimeManager* _timer;
	//���ł�
	GimmickFactory* _fac;
	EnemyFactory* _emFac;

	const char* mapName;

	RESULT_DATA _rtData;

	int count;			//�J�ڊm�F�p�i���Ƃō폜���܂��j
	
	void GameInit();											//�Q�[���J�n���̏��������s���܂�

	void NormalUpdata(Input* input);							//�S�̂�Updata
	void ObjectUpdata(Input* input,Position2& offset);			//�e��׸���Ȃǂ�Updata���Ăяo��
	void UsingRopeUpdata(Input* input,Position2& offset);		//۰�߂��g�p���Ă���Ƃ���Updata���Ăяo��
	void TransitionUpdata(Input* input);						//���Q���������ʑJ�ڂ��s���\��
	void Draw(Position2& offset);
public:
	GameScene();
	~GameScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);

};

