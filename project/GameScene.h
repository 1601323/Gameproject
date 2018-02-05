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
class Input;

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
	int numberImage;
	int second;
	int tenex;
	int hunex;
	int lightImage;
	int pauseNowNum;                                            //���ݑI�����Ă���|�[�Y�̃��[�h
	int PauseDirNumY;
	int dirMoveCnt;
	bool selectPauseFlag;

	RESULT_DATA _rtData;
	FEVER_DATA _feverData;
	SENSING_VALUE _minSensingValueL;

	int count;			//�J�ڊm�F�p�i���Ƃō폜���܂��j
	void GameInit();											//�Q�[���J�n���̏��������s���܂�
	void NormalUpdata(Input* input);							//�S�̂�Updata
	void ObjectUpdata(Input* input,Position2& offset);			//�e��׸���Ȃǂ�Updata���Ăяo��
	void UsingRopeUpdata(Input* input,Position2& offset);		//۰�߂��g�p���Ă���Ƃ���Updata���Ăяo��
	void FadeInUpdata(Input* intput);							//�t�F�[�h�C�����s���܂�
	void TransitionUpdata(Input* input);						//���Q���������ʑJ�ڂ��s���\��
	void PauseUpdata(Input* input);
	void PauseSelect(Input* input);

	void RetryProcess();
	void GameScene::RetryPauseProcess();
	void UpdateManager();											//updata�̂��тɌĂяo���֐����܂Ƃ߂Ă���
	void Draw(Position2& offset);
	void DrawPauseUi(void);
	void DrawUI();	
	void DrawBack(Position2 offset);
	void JudgeTransition();
public:
	GameScene();
	~GameScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

