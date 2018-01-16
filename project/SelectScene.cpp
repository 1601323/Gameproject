#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "SelectScene.h"
#include "Input.h"
#include "TitleScene.h"
#include "GameScene.h"


SelectScene::SelectScene()
{
	_updater = &SelectScene::NormalUpdata;
}


SelectScene::~SelectScene()
{
}
void SelectScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	_inpInfo = input->GetInput(1);
	Select();
#ifdef _DEBUG
	DrawString(10,0,"セレクト",GetColor(255,255,255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.Instance().ChangeScene(new GameScene());
	}
}
SCENE_TYPE SelectScene::GetScene()
{
	return SCENE_SELECT;
}
void SelectScene::Updata(Input* input)
{
	(this->*_updater)(input);
}

void SelectScene::Select()
{
	//ステージ選択
	if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !lastKey.keybit.R_RIGHT_BUTTON) {
		nowNum += 1;
		if (nowNum >= 6) {
			nowNum = 0;
		}
	}
	else if (_inpInfo.key.keybit.R_LEFT_BUTTON && !lastKey.keybit.R_LEFT_BUTTON) {
		nowNum -= 1;
		if (nowNum < 0) {
			nowNum = 5;
		}
	}
	else if (_inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
		nowNum += 3;
		if (nowNum >= 6) {
			nowNum -= 6;
		}
	}
	else if (_inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
		nowNum -= 3;
		if (nowNum < 0) {
			nowNum += 6;
		}
	}
	else {
	}
}