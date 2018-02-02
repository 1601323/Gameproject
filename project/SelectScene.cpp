#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "ImageMgr.h"
#include "GameMain.h"
#include "SelectScene.h"
#include "Input.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ModelMgr.h"
using namespace std;


SelectScene::SelectScene()
{
	_updater = &SelectScene::NormalUpdata;
	_modelmgr = ModelMgr::Instance();
	SelectMap = mapNumber[0];
	nowNum = 0;
	_minSensingValueL = SV_HIGH;
	selectFlag = false;
	w = 90;
	h = 60;
	stageNum[0] = "仮image/stNum1.png";
	stageNum[1] = "仮image/stNum2.png";
	stageNum[2] = "仮image/stNum3.png";
	modelhandle = MV1LoadModel("player_model/player.pmx");
	//アニメーションをアタッチ+総時間の設定
	AnimIndex = MV1AttachAnim(modelhandle, ACTION_WAIT, -1, false);
	AnimTotalTime = MV1GetAttachAnimTotalTime(modelhandle, AnimIndex);
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
	Select(input);
#ifdef _DEBUG
	//DrawString(10, 0, "セレクト", GetColor(255, 255, 255));
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
void SelectScene::Select(Input* input)
{
	if (_inpInfo.num >= 1) {
		if ((input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
			selectFlag == false) {
			nowNum += 1;
			if (nowNum >= STAGE_MAX) {
				nowNum = 0;
			}
			selectFlag = true;
		}
		else if ((input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_LEFT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
			selectFlag == false) {
			nowNum -= 1;
			if (nowNum < 0) {
				nowNum = STAGE_MAX - 1;
			}
			selectFlag = true;
		}
		else if (!((input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
			!((input->GetStickDir(_inpInfo.L_Stick.lstick) == SD_LEFT) &&
				_inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
			selectFlag = false;
		}
		else {
			nowNum = nowNum;
		}
	}
	else {

		//ステージ選択
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !lastKey.keybit.R_RIGHT_BUTTON) {
			nowNum += 1;
			if (nowNum >= STAGE_MAX) {
				nowNum = 0;
			}
		}
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON && !lastKey.keybit.R_LEFT_BUTTON) {
			nowNum -= 1;
			if (nowNum < 0) {
				nowNum = STAGE_MAX - 1;
			}
		}
		else {
			nowNum = nowNum;
		}
	}
}
void SelectScene::Draw()
{
	ImageMgr& im = ImageMgr::Instance();

	int redu = 40;
	//背景
	DrawGraph(0, 0, im.ImageIdReturn("仮image/select.png",SCENE_GAME),true);


	//ステージ選択用の四角
	//for (int x = 0; x < 3; x++) {
	//	DrawBox(90 + 100 * x, 90  , 90 + 100 * x + w, 90  + h, 0x223344, true);
	//}
	for (int f = 0; f < STAGE_MAX; f++) {
		if (f == nowNum) {	//選ばれていたら拡大表示
			DrawExtendGraph(10 + 260 * f, 60, 10 + 260 + 260 * f, 60 + 260, im.ImageIdReturn(stageNum[f], SCENE_GAME), true);
			DrawExtendGraph(10 + 260 * f, 60 , 10 + 260 + 260 * f, 60 + 260 , im.ImageIdReturn("仮image/stage.png", SCENE_GAME), true);
		}
		else
		{
			DrawExtendGraph(10 + 260 * f + redu, 60 + redu, 10 + 260 + 260 * f - redu, 60 + 260 - redu, im.ImageIdReturn(stageNum[f], SCENE_GAME), true);
			DrawExtendGraph(10+ 260*f+redu, 60+redu ,10+260+ 260*f-redu,60+260 -redu,im.ImageIdReturn("仮image/stage.png", SCENE_GAME), true);
		}
	}

	DrawExtendGraph(180, 450,750,620, im.ImageIdReturn("仮image/textbox.png",SCENE_GAME), true);

	//アニメーションのフレームを進める
	AnimNowTime += 0.5f;
	//現在のアニメーションが最大フレームまでいったらループする
	if (AnimNowTime >= AnimTotalTime)
	{
		AnimNowTime = 0;
	}
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, AngleRad(-30.f), 0.f));
	//アニメーションをアタッチ
	MV1SetAttachAnimTime(modelhandle, AnimIndex, AnimNowTime);
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(100.f, 600, 0.f)));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(3.5f, 3.5f, 3.5f));
	//モデルの輪郭線を設定 0.0fで透過します
	_modelmgr->SetMaterialDotLine(modelhandle, 0.0f);
	//モデルを描画
	MV1DrawModel(modelhandle);


//	DrawBox(90 + 100 * (nowNum % 3), 90 , 90 + 100 * (nowNum % 3) + w, 90 + h, 0x999999, true);
	DrawFormatString(190,470,0x000000,"Stage %d です",nowNum+1);
}