#pragma once
#include "Scene.h"
class Input;

class TitleScene :
	public Scene
{
private:
	void(TitleScene::*_updater)(Input* input);

	void NormalUpdata(Input* input);
public:
	TitleScene();
	~TitleScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

