#pragma once
#include "Scene.h"

#define COLOR_NUM (40)

class Input;
class ModelMgr;

enum SENSING_VALUE;
class SelectScene :
	public Scene
{
private:
	void(SelectScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
	void Draw();
	void Select(Input* input);
	ModelMgr* _modelmgr;

	KEY key;
	KEY lastKey;
	INPUT_INFO _inpInfo;
	SENSING_VALUE _minSensingValueL;

	int nowNum;
	int mapNumber[STAGE_MAX];
	int SelectMap;
	int modelhandle;        //プレイヤーモデルハンドル
	int colorNum;           //カラー変更タイマー
	int  AnimIndex;         //アニメーション数
	float AnimTotalTime;    //各アニメーションのトータルタイム
	float AnimNowTime;      //各アニメーションの現在タイム
	bool selectFlag;		//セレクトのキーの制御フラグ

	int w;
	int h;
	char* stageNum[3];
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

