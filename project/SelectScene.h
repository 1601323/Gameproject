#pragma once
#include "Scene.h"
class Input;
enum SENSING_VALUE;
class SelectScene :
	public Scene
{
private:
	void(SelectScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
	void Draw();
	void Select(Input* input);

	KEY key;
	KEY lastKey;
	INPUT_INFO _inpInfo;
	SENSING_VALUE _minSensingValueL;

	int nowNum;
	int mapNumber[STAGE_MAX];
	int SelectMap;
	bool selectFlag;		//セレクトのキーの制御フラグ

	int w;
	int h;
	char* stageImage[3];
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

