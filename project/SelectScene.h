#pragma once
#include "Scene.h"
class Input;

class SelectScene :
	public Scene
{
private:
	void(SelectScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
<<<<<<< HEAD
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
=======
>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

