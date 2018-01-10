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

	KEY key ;
	KEY lastKey ;
	INPUT_INFO _inpInfo;
	int nowNum;

	//確認用（あとで削除）
	int w;
	int h;
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
	//まだ構成がはっきりしていなのでここで設定しておく
	int SelectMap;
	int mapNumber[6];
};

