#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Input.h"
#include "GameMain.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "ResultScene.h"



ResultScene::ResultScene()
{
	_updater = &ResultScene::NormalUpdata;
}

ResultScene::~ResultScene()
{
}

void ResultScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

	if (gm.GetResultData().goalFlag == true) {
		GameClear();
	}
	else {
		GameOver();
	}
#ifdef _DEBUG
	DrawString(10, 10, "リザルトに遷移してるよ！", 0xffffff);
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.Instance().ChangeScene(new GameScene());
	}
}
//クリアとゲームオーバーに分けておく
void ResultScene::GameClear()
{
	GameMain& gm = GameMain::Instance();
#ifdef _DEBUG
	DrawString(100,100,"Clear",0xff00ff);
#endif
}
void ResultScene::GameOver()
{
#ifdef _DEBUG
	DrawString(100, 100, "GameOver", 0xff00ff);
#endif
}
SCENE_TYPE ResultScene::GetScene()
{
	return SCENE_RESULT;
}
void ResultScene::Updata(Input* input)
{
	(this->*_updater)(input);
}