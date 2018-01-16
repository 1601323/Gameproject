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
	DrawString(10,0,"ƒZƒŒƒNƒg",GetColor(255,255,255));
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

}