#include <DxLib.h>
#include "GameMain.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "SelectScene.h"
#include "Input.h"

//
//���ݎg�p���Ă��܂���(ResultScene�ɓ���)
//
//
ClearScene::ClearScene()
{
	_updater = &ClearScene::NormalUpdata;
}


ClearScene::~ClearScene()
{
}

void ClearScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

#ifdef _DEBUG
	DrawString(10,0,"�N���A",GetColor(255,255,255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		gm.Instance().ChangeScene(new GameScene());
	}
	if (key.keybit.B_BUTTON && !lastKey.keybit.B_BUTTON) {
		gm.Instance().ChangeScene(new SelectScene());
	}
}

SCENE_TYPE ClearScene::GetScene()
{
	return SCENE_CLEAR;
}
void ClearScene::Updata(Input* input)
{
	(this->*_updater)(input);
}