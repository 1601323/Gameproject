#pragma once
#include "Scene.h"

class Input;

class ClearScene :
	public Scene
{
private:
	void(ClearScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
public:
	ClearScene();
	~ClearScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

