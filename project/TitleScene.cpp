#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "TitleScene.h"
#include "Input.h"
#include "SelectScene.h"


TitleScene::TitleScene()
{
	_updater = &TitleScene::NormalUpdata;
}


TitleScene::~TitleScene()
{
}
void TitleScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);
#ifdef _DEBUG
	DrawString(10,0,"ƒ^ƒCƒgƒ‹",GetColor(255,255,255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.Instance().ChangeScene(new SelectScene());
	}
}

SCENE_TYPE TitleScene::GetScene()
{
	return SCENE_TITLE;
}
void TitleScene::Updata(Input* input)
{
	(this->*_updater)(input);
}