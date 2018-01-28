#include <DxLib.h>
#include "Input.h"
#include "GameMain.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "SelectScene.h"

//
//現在使用していません(ResultSceneに統合)
//
//

GameOverScene::GameOverScene()
{
	_updater = &GameOverScene::NormalUpdata;
}


GameOverScene::~GameOverScene()
{
}

void GameOverScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);
#ifdef _DEBUG
	//DrawString(10,0,"ゲームオーバー",GetColor(255,255,255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.Instance().ChangeScene(new GameScene());
	}
	if (key.keybit.B_BUTTON && !lastKey.keybit.B_BUTTON) {
		gm.Instance().ChangeScene(new SelectScene());
	}
}

SCENE_TYPE GameOverScene::GetScene()
{
	return SCENE_GAMEOVER;
}
void GameOverScene::Updata(Input* input)
{
	(this->*_updater)(input);
}