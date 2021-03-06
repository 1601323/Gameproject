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
class Tutorial;

class ModelManager;

class GimmickFactory;
class EnemyFactory;


class GameScene :public Scene
{
private:
	void(GameScene::*_updater)(Input* input);		//シーン遷移用
	Player* _player;
	Enemy* _enemy;
	Camera* _cam;
	Rope* _rope;	
	HitClass* _hit;
	MapCtl* _map;
	EnemyServer* _server;
	Midpoint* _mid;
	TimeManager* _timer;
	Tutorial* _tuto;
	//仮です
	GimmickFactory* _fac;
	EnemyFactory* _emFac;

	const char* mapName;
	int numberImage;
	int second;
	int tenex;
	int hunex;
	int lightImage;
	int pauseNowNum;                                            //現在選択しているポーズのモード
	int pauseRetireNowNum;                                      //ポーズでのリタイアで選択しているモード
	int pauseDirNumX;
	int pauseDirNumY;
	int dirMoveCnt;
	int BgImage;
	bool selectPauseFlag;

	bool ChackFlag;                                             //ポーズの確認用
	bool ChackDrawFlag;                                         //ポーズの確認描画用
	bool RetryOrRetireFlag;                                     //描画をリタイアかリトライどちらかを決めるflag true:リトライ false:リザルト

	bool bgmFlag;												//BGMがなっているかどうかのフラグ


	RESULT_DATA _rtData;
	FEVER_DATA _feverData;
	SENSING_VALUE _minSensingValueL;

	int count;			//遷移確認用（あとで削除します）
	void GameInit();											//ゲーム開始時の初期化を行います
	void NormalUpdata(Input* input);							//全体のUpdata
	void ObjectUpdata(Input* input,Position2& offset);			//各ｷｬﾗｸﾀｰなどのUpdataを呼び出す
	void UsingRopeUpdata(Input* input,Position2& offset);		//ﾛｰﾌﾟを使用しているときのUpdataを呼び出す
	void FadeInUpdata(Input* intput);							//フェードインを行います
	void TransitionUpdata(Input* input);						//仮＿ここから画面遷移を行う予定
	void TutorialUpdata(Input* input, Position2& offset);       //チュートリアル状態のときに呼ばれる
	void PauseUpdata(Input* input);
	void PauseSelect(Input* input);
	void CheckReTireSelect(Input* input);                        //リタイア用


	void RetryProcess();
	void GameScene::RetryPauseProcess();
	void UpdateManager();											//updataのたびに呼び出す関数をまとめておく
	void Draw(Position2& offset);
	void DrawPauseUi(void);
	void DrawUI();	
	void DrawCheckUi();
	void DrawBack(Position2 offset);
	void JudgeTransition();
public:
	GameScene();
	~GameScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

