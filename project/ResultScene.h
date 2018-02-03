#pragma once
#include "Scene.h"

class Input;
class ModelMgr;

enum NEXT_MODE {
	JUMP_RETRY,
	JUMP_TITLE,
	JUMP_SELECT,
	JUMP_MAX
};

class ResultScene :
	public Scene
{
private:
	RESULT_DATA _rtData;
	void(ResultScene::*_updater)(Input* input);
	KEY key;
	KEY lastKey;
	INPUT_INFO inpInfo;
	NEXT_MODE _mode;
	SENSING_VALUE _minSensingValueL;
	NEXT_MODE mode[JUMP_MAX];
	int nowNum;
	//表示用関数
	int numberImage;
	int second;
	int tenex;
	int hunex;

	int dirNumY;
	bool clearFlag;
	bool selectFlag;

	ModelMgr* _modelmgr;
	int playerModelHandle;
	int smileTexture;
	int sadTexture;
	int textureIndex[2];
	int medicineHandle;

	float AnimTotalTimeH;           //アニメーション総時間 HはhappyのH
	float AnimNowTimeH;             //今のアニメーション時間 HはhappyのH
	int  AnimIndexH;                //アニメーションインデックス HはhappyのH

	float AnimTotalTimeS;           //アニメーション総時間 SはhappyのS
	float AnimNowTimeS;             //今のアニメーション時間 SはhappyのS
	int  AnimIndexS;                //アニメーションインデックス SはhappyのS

	void NormalUpdata(Input* input);

	void GameClear();
	void GameOver();
	void Select(Input* input);
	void Draw();
public:
	ResultScene();
	~ResultScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};