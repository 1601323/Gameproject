#pragma once
#include "Scene.h"

class Input;

class ResultScene :
	public Scene
{
private:
	RESULT_DATA _rtData;
	void(ResultScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);

	void GameClear();
	void GameOver();
public:
	ResultScene();
	~ResultScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

