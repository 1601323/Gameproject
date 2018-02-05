
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
	void FadeinTitle(Input* input);
	void TitleMovie(Input* input);
	void NormalUpdata(Input* input);
	void FadeoutTitle(Input* input);
	void MenuSelect(Input* input);
	void scroll();
	void Draw();
	void InitMovie();
	SENSING_VALUE _minSensingValueL;

	bool titleFlag;		//タイトル画面（Aを押してね）
	bool selectFlag;	//メニューを表示　
	int noInputTime;
	int menuCnt;

	int skyImage;
	int dirMoveCnt;
	Position2 _skyPos[2];		//背景スクロール用
	char* uiMovie;
	bool movieFlag;

	bool initFlag;

	int lightCnt;
	int flameCnt;
public:
	TitleScene();
	~TitleScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};
