#pragma once
#include "Scene.h"

enum TITLE_MENU {
	GAME_START,
	GAME_EXPLAIN,
	MENU_MAX
};
class Input;

class TitleScene :
	public Scene
{
private:
	KEY key;
	KEY lastKey;
	INPUT_INFO inpInfo;
	void(TitleScene::*_updater)(Input* input);
	TITLE_MENU _menu;
	void NormalUpdata(Input* input);
	void MenuSelect();
public:
	TitleScene();
	~TitleScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

