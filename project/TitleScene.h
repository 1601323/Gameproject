#pragma once
#include "Scene.h"

enum TITLE_MENU {
	GAME_START,
	GAME_EXPLAIN,
	MENU_MAX
};
class Input;

enum SENSING_VALUE;

class TitleScene :
	public Scene
{
private:
	KEY key;
	KEY lastKey;
	INPUT_INFO inpInfo;
	void(TitleScene::*_updater)(Input* input);
	TITLE_MENU _menu;
	bool operateFlag;
	void NormalUpdata(Input* input);
	void MenuSelect(Input* input);
	void Draw();
	SENSING_VALUE _minSensingValueL;
public:
	TitleScene();
	~TitleScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

