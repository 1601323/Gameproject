#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "TitleScene.h"
#include "Input.h"
#include "SelectScene.h"


TitleScene::TitleScene()
{
	_updater = &TitleScene::NormalUpdata;
	_menu = GAME_START;
	_minSensingValueL = SV_HIGH;
	operateFlag = false;
}


TitleScene::~TitleScene()
{
}
void TitleScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	if (operateFlag == false) {
		MenuSelect(input);
	}
	Draw();
#ifdef _DEBUG
	DrawString(10,0,"タイトル",GetColor(255,255,255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (_menu == GAME_START) {
			gm.Instance().ChangeScene(new SelectScene());
		}
		else if (_menu == GAME_EXPLAIN) {
			operateFlag = !operateFlag;
		}
	}
}
void TitleScene::MenuSelect(Input* input)
{
	if (inpInfo.num >= 1) {
		if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_menu = GAME_EXPLAIN;
			//if (_menu == GAME_START) {
			//	_menu =GAME_EXPLAIN;
			//}
		}
		else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_LEFT) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_menu = GAME_START;
			//if (_menu == GAME_EXPLAIN) {
			//	_menu = GAME_START;
			//}
		}
		else {
			_menu = _menu;
		}
	}
	else {	//キーボードの場合
		if (inpInfo.key.keybit.R_RIGHT_BUTTON && !lastKey.keybit.R_RIGHT_BUTTON) {
			_menu = GAME_EXPLAIN;
			//if (_menu == GAME_START) {
			//	_menu =GAME_EXPLAIN;
			//}
		}
		else if (inpInfo.key.keybit.R_LEFT_BUTTON && !lastKey.keybit.R_LEFT_BUTTON) {
			_menu = GAME_START;
			//if (_menu == GAME_EXPLAIN) {
			//	_menu = GAME_START;
			//}
		}
		else {
			_menu = _menu;
		}
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
void TitleScene::Draw()
{
	DrawBox(200,200,300,300,0xffaaff,true);
	if (_menu == GAME_START) {
		DrawBox(200, 200, 300, 300, 0xffffff, true);
	}
	DrawString(250,250,"Game",0xffff99);

	DrawBox(400, 200, 500, 300, 0xffaaff, true);
	if (_menu == GAME_EXPLAIN) {
		DrawBox(400, 200, 500, 300, 0xffffff, true);
	}
	DrawString(450,250,"explain",0xffff99);

	if (operateFlag == true) {
		DrawBox(200, 400, 500, 500, 0xffffff, true);
		DrawString(220,410,"未実装",0x000000);
		DrawString(250,450,"もう一度エンターで選択を解除",0x000000);
	}
}