#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "GameMain.h"
#include "SelectScene.h"
#include "Input.h"
#include "TitleScene.h"
#include "GameScene.h"
using namespace std;


SelectScene::SelectScene()
{
	_updater = &SelectScene::NormalUpdata;
	SelectMap = mapNumber[0];
	nowNum = 0;

	w = 90;
	h = 60;
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
	DrawString(10, 0, "セレクト", GetColor(255, 255, 255));
#endif
	Draw();

	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.SetNowStage(nowNum);
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
void SelectScene::Draw()
{
	//背景色
	DrawBox(0, 0, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT, 0xffffff, true);
	//ステージ選択用の四角
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 3; x++) {
			DrawBox(90 + 100 * x, 90 + 100 * y, 90 + 100 * x + w, 90 + 100 * y + h, 0x223344, true);
		}
	}
	DrawBox(90 + 100 * (nowNum % 3), 90 + 100 * (nowNum / 3), 90 + 100 * (nowNum % 3) + w, 90 + 100 * (nowNum / 3) + h, 0x999999, true);
	//cout << nowNum << endl;
}