#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Input.h"
#include "GameMain.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"



ResultScene::ResultScene()
{
	_updater = &ResultScene::NormalUpdata;
	_minSensingValueL = SV_HIGH;
	_mode = JUMP_SELECT;
	clearFlag = false;
	selectFlag = false;
	nowNum = 0;
}

ResultScene::~ResultScene()
{
}

void ResultScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	_rtData = gm.GetResultData();
	if (gm.GetResultData().goalFlag == true && gm.GetResultData().midFlag) {
		GameClear();
		clearFlag = true;
	}
	else {
		GameOver();
		clearFlag = false;
	}
	Select(input);
	Draw();
#ifdef _DEBUG
	DrawString(10, 10, "リザルトに遷移してるよ！", 0xffffff);
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (nowNum == JUMP_SELECT) {
			gm.Instance().ChangeScene(new SelectScene());
		}
		else if (nowNum == JUMP_TITLE) {
			gm.Instance().ChangeScene(new TitleScene());
		}
		else if (nowNum== JUMP_RETRY) {
			gm.Instance().ChangeScene(new GameScene());
		}
		else {
		}
	}
}
//クリアとゲームオーバーに分けておく
void ResultScene::GameClear()
{
	GameMain& gm = GameMain::Instance();
	gm.NewDataSet();
	gm.BestDataSet();
	DrawFormatString(100,150,0xffffff,"%d",_rtData.goalTime);
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
void ResultScene::Select(Input*  input)
{
	if (clearFlag == true) {
		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX -1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 1;
				}
				selectFlag = true;
			}
			else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
				!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
					inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
				selectFlag = false;
			}
			else {
				nowNum = nowNum;
			}
		}
		else {

			//ステージ選択
			if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
				nowNum--;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX - 1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 1;
				}
			}
			else {
			}
		}
	}
	else if (clearFlag == false) {
		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum =0;
				}
				selectFlag = true;
			}
			else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
				!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
					inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
				selectFlag = false;
			}
			else {
				nowNum = nowNum;
			}
		}
		else {

			//ステージ選択
			if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
				nowNum--;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 0;
				}
			}
			else {
			}
		}
	}
	else {
	}
}
void ResultScene::Draw()
{
	if (clearFlag == true) {
		DrawString(300, 300, "セレクト", 0xffffff);
		DrawString(300, 320, "タイトル", 0xffffff);
	}
	else if (clearFlag == false) {
		DrawString(300,280,"リトライ",0xffffff);
		DrawString(300, 300, "セレクト", 0xffffff);
		DrawString(300, 320, "タイトル", 0xffffff);
	}
	switch (nowNum) {
	case 0:
		DrawString(280, 280, "→", 0xffffff);
		break;
	case 1:
		DrawString(280, 300, "→", 0xffffff);
		break;
	case 2:
		DrawString(280, 320, "→", 0xffffff);
		break;
	default:
		break;
	}
}
SCENE_TYPE ResultScene::GetScene()
{
	return SCENE_RESULT;
}
void ResultScene::Updata(Input* input)
{
	(this->*_updater)(input);
}
