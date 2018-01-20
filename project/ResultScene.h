#pragma once
#include "Scene.h"

class Input;
enum NEXT_MODE {
	JUMP_RETRY,
	JUMP_SELECT,
	JUMP_TITLE,
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
	bool clearFlag;
	bool selectFlag;
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

