#pragma once
#include "Scene.h"

class Input;

class GameOverScene :
	public Scene
{
private:
	void(GameOverScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
public:
	GameOverScene();
	~GameOverScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

