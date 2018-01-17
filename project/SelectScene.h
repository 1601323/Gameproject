#pragma once
#include "Scene.h"
class Input;

class SelectScene :
	public Scene
{
private:
	void(SelectScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
	void Draw();
	void Select();

	KEY key;
	KEY lastKey;
	INPUT_INFO _inpInfo;

	int nowNum;
	int mapNumber[STAGE_MAX];
	int SelectMap;

	int w;
	int h;
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

