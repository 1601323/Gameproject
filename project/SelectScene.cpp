#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "ImageMgr.h"
#include "GameMain.h"
#include "SelectScene.h"
#include "Input.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SoundMgr.h"
#include "ModelMgr.h"
using namespace std;


SelectScene::SelectScene()
{
	_updater = &SelectScene::NormalUpdata;
	_modelmgr = ModelMgr::Instance();
	SelectMap = mapNumber[0];
	nowNum = 0;
	dirMoveCnt = 0;
	_minSensingValueL = SV_HIGH;
	selectFlag = false;
	w = 90;
	h = 60;
	stageNum[0] = "image/ステージサムネイル/stage1.png";
	stageNum[1] = "image/ステージサムネイル/stage2.png";
	stageNum[2] = "image/ステージサムネイル/stage3.png";
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
		colorNum = 0;
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
	SoundMgr& so = SoundMgr::Instance();
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
			so.SeStart("Bgm/ok.mp3", SCENE_GAME);
		}
	}
	else {

		//ステージ選択
		if (_inpInfo.key.keybit.R_RIGHT_BUTTON && !lastKey.keybit.R_RIGHT_BUTTON) {
			nowNum += 1;
			if (nowNum >= STAGE_MAX) {
				nowNum = 0;
			}
			so.SeStart("Bgm/cursorMove.mp3", SCENE_GAME);
		}
		else if (_inpInfo.key.keybit.R_LEFT_BUTTON && !lastKey.keybit.R_LEFT_BUTTON) {
			nowNum -= 1;
			if (nowNum < 0) {
				nowNum = STAGE_MAX - 1;
			}
			so.SeStart("Bgm/cursorMove.mp3", SCENE_GAME);
		}
		else {
			nowNum = nowNum;
			so.SeStart("Bgm/ok.mp3", SCENE_GAME);
		}
	}
}
void SelectScene::Draw()
{
	ImageMgr& im = ImageMgr::Instance();
	++colorNum;
	colorNum %= COLOR_NUM;
	dirMoveCnt++;
	int redu = 40;
	//背景
	DrawGraph(0, 0, im.ImageIdReturn("image/select.png",SCENE_GAME),true);

	//ステージ選択用の四角
	//for (int x = 0; x < 3; x++) {
	//	DrawBox(90 + 100 * x, 90  , 90 + 100 * x + w, 90  + h, 0x223344, true);
	//}
	for (int f = 0; f < STAGE_MAX; f++) {
		if (f == nowNum) {	//選ばれていたら拡大表示
			DrawExtendGraph(28 + 260 * f, 82, 245 + 260 * f, 40 + 255, im.ImageIdReturn(stageNum[f], SCENE_GAME), true);

			SetDrawBright(243, 152, 0);
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255 * (static_cast<float>(abs(20 - colorNum)) / static_cast<float>((COLOR_NUM / 1.5f))));
			DrawExtendGraph(10 + 260 * f, 60 , 10 + 260 + 260 * f, 60 + 260 , im.ImageIdReturn("image/stage.png", SCENE_GAME), true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			SetDrawBright(255,255,255);

			//矢印
			DrawGraph(200 + abs(30 - (200 + (dirMoveCnt / 2 % 60)) % 59) + 260 * f,160, im.ImageIdReturn("image/yazirushi2.png", SCENE_GAME),true);
			DrawTurnGraph(10 - abs(30 - (200 + (dirMoveCnt / 2 % 60)) % 59) + 260 * f, 160, im.ImageIdReturn("image/yazirushi2.png", SCENE_GAME), true);

		}
		else
		{
			DrawExtendGraph(23 + 260 * f + redu, 72 + redu, 255 + 260 * f - redu, 45 + 260 - redu, im.ImageIdReturn(stageNum[f], SCENE_GAME), true);

			DrawExtendGraph(10+ 260*f+redu, 60+redu ,10+260+ 260*f-redu,60+260 -redu,im.ImageIdReturn("image/stage.png", SCENE_GAME), true);
		}
	}

	DrawExtendGraph(180, 450,750,620, im.ImageIdReturn("image/textbox.png",SCENE_GAME), true);

	//アニメーションのフレームを進める
	AnimNowTime += 0.5f;
	//現在のアニメーションが最大フレームまでいったらループする
	if (AnimNowTime >= AnimTotalTime)
	{
		AnimNowTime = 0;
	}
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.f, AngleRad(-5.f), 0.f));
	//アニメーションをアタッチ
	MV1SetAttachAnimTime(modelhandle, AnimIndex, AnimNowTime);
	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(100.f, 600, 0.0f)));
	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(3.5f, 3.5f, 3.5f));
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(modelhandle, 0.0f);


	switch (nowNum)
	{
	case 0:
		DrawGraph(200, 480, im.ImageIdReturn("image/Select/Stage1.png", SCENE_GAME), true);
		break;
	case 1:
		DrawGraph(200, 480, im.ImageIdReturn("image/Select/Stage2.png", SCENE_GAME), true);
		break;
	case 2:
		DrawGraph(200, 480, im.ImageIdReturn("image/Select/Stage3.png", SCENE_GAME), true);
		break;
	default:
		break;
	}

//	DrawBox(90 + 100 * (nowNum % 3), 90 , 90 + 100 * (nowNum % 3) + w, 90 + h, 0x999999, true);
//	DrawFormatString(190,470,0x000000,"Stage %d です",nowNum+1);
}