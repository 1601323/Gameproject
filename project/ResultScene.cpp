#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Input.h"
#include "GameMain.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "ModelMgr.h"
#include "ImageMgr.h"

#define NUM_X (74)
#define NUM_Y (100)
#define DELAY_TIMER (50)//GAMEOVER文字が落ちてくる時間の感覚

ResultScene::ResultScene()
{
	_updater = &ResultScene::NormalUpdata;
	_minSensingValueL = SV_HIGH;
	_mode = JUMP_SELECT;
	clearFlag = false;
	selectFlag = false;
	nowNum = 0;
	dirMoveCnt = 0;
	AnnouncCnt = 0;
	AnimNowTimeH = 0.f;
	AnimNowTimeS = 0.f;
	LogoDownCounter = -100;
	_modelmgr = ModelMgr::Instance();
	//モデル読み込み
	playerModelHandle = MV1LoadModel("player_model/player.pmx");
	//playerModelWithFlask = _modelmgr->ModelIdReturn("player clear _model/player clear.pmx", SCENE_TITLE);
	playerModelWithFlask = MV1LoadModel("player clear _model/player clear.pmx");

	//アニメーションをアタッチ+総時間の設定
	//クリア時
	AnimIndexH = MV1AttachAnim(playerModelWithFlask,0, -1, false);
	AnimTotalTimeH = MV1GetAttachAnimTotalTime(playerModelWithFlask, AnimIndexH);
	//ゲームオーバー時
	AnimIndexS = MV1AttachAnim(playerModelHandle, ACTION_AOONI, -1, false);
	AnimTotalTimeS = MV1GetAttachAnimTotalTime(playerModelHandle, AnimIndexS);
	//顔のテクスチャのindexを取得
	textureIndex = MV1GetMaterialDifMapTexture(playerModelHandle, 1);
	textureIndexFlask = MV1GetMaterialDifMapTexture(playerModelWithFlask, 1);
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
		clearFlag = true;
	}

	//ロゴがすべて落ちるまで処理しない
	if (LogoDownCounter >= 500 )
	{
		Select(input);
#ifdef _DEBUG
		//DrawString(10, 10, "リザルトに遷移してるよ！", 0xffffff);
#endif
		if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
			if (nowNum == JUMP_SELECT) {
				dirMoveCnt = 0;
				LogoDownCounter = -100;
				gm.Instance().ChangeScene(new SelectScene());
			}
			else if (nowNum == JUMP_TITLE) {
				dirMoveCnt = 0;
				LogoDownCounter = -100;
				gm.Instance().ChangeScene(new TitleScene());
			}
			else if (nowNum == JUMP_RETRY) {
				dirMoveCnt = 0;
				LogoDownCounter = -100;
				gm.Instance().ChangeScene(new GameScene());
			}
			else {
			}

		}
	}
	Draw();
}
//クリアとゲームオーバーに分けておく
void ResultScene::GameClear()
{
	GameMain& gm = GameMain::Instance();
	gm.NewDataSet();
	gm.BestDataSet();
	//DrawFormatString(100, 150, 0xffffff, "%d", _rtData.goalTime);

#ifdef _DEBUG
	//DrawString(100, 100, "Clear", 0xff00ff);
#endif
}
void ResultScene::GameOver()
{
#ifdef _DEBUG
	//DrawString(100, 100, "GameOver", 0xff00ff);
#endif
}
void ResultScene::Select(Input*  input)
{
	if (clearFlag == true) {
		if (nowNum <= 0) nowNum = 1;//クリアの場合初期nowNumを1にしとく

		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				dirMoveCnt = 0;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX - 1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				dirMoveCnt = 0;
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
				dirMoveCnt = 0;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX-1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				dirMoveCnt = 0;
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
				dirMoveCnt = 0;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				dirMoveCnt = 0;

				if (nowNum >= JUMP_MAX) {
					nowNum = 0;
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
				dirMoveCnt = 0;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				dirMoveCnt = 0;
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
	ImageMgr& im = ImageMgr::Instance();
	dirMoveCnt++;
	LogoDownCounter += 3;

	//AnnouncCnt++;
	//if (AnnouncCnt > 60 * 3)//3秒で結果を出す
	//{
	//	AnnouncCnt = 0;
	//}

	//クリアしたら
	if (clearFlag == true) {
		//背景
		DrawGraph(0, 0, im.ImageIdReturn("image/Clear/Clear.png", SCENE_TITLE), true);

		//プレイヤー happy
		//アニメーションのフレームを進める
		AnimNowTimeH += 0.5f;
		//現在のアニメーションが最大フレームまでいったらループする
		if (AnimNowTimeH >= AnimTotalTimeH)
		{
			AnimNowTimeH = 0;
		}
		//モデルの回転角度の設定(ラジアン)
		MV1SetRotationXYZ(playerModelWithFlask, VGet(0.f,0.f, 0.f));
		//アニメーションをアタッチ
		MV1SetAttachAnimTime(playerModelWithFlask, AnimIndexH, AnimNowTimeH);
		//モデルのposを設定+ワールド座標からスクリーンへ変換
		MV1SetPosition(playerModelWithFlask, ConvWorldPosToScreenPos(VGet(200.f, 600, 0.f)));
		//モデルの拡大縮小値の設定
		MV1SetScale(playerModelWithFlask, VGet(4.0f, 4.0f, 4.0f));
		//顔のテクスチャを笑顔の方に変更
		MV1SetTextureGraphHandle(playerModelWithFlask, textureIndexFlask, im.ImageIdReturn("player_model/face2.png", SCENE_TITLE), FALSE);
		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(playerModelWithFlask, 0.0f);

		//clear文字
		DrawGameClearLogo();
		//スコアタイムの画像読み込みと表示
		DrawGoalTimer();
		
	}
	else if (clearFlag == false) {
		//背景
		DrawGraph(0, 0, im.ImageIdReturn("image/Over/GameOver.png", SCENE_TITLE), true);
		//プレイヤー
		AnimNowTimeS += 0.5f;
		if (AnimNowTimeS >= AnimTotalTimeS)
		{
			AnimNowTimeS = 0;
		}
		MV1SetAttachAnimTime(playerModelHandle, AnimIndexS, AnimNowTimeS);
		MV1SetPosition(playerModelHandle, ConvWorldPosToScreenPos(VGet(300.f, 580, 0.f)));
		MV1SetScale(playerModelHandle, VGet(5.0f, 5.0f, 5.0f));
		MV1SetTextureGraphHandle(playerModelHandle, textureIndex, im.ImageIdReturn("player_model/cryFace.png", SCENE_TITLE), FALSE);

		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(playerModelHandle, 0.0f);

		DrawGraph(0, 0, im.ImageIdReturn("image/Over/Fence.png", SCENE_TITLE), true);
		//gameover文字
		DrawGameOverLogo();
	}

	//ロゴがすべて落ちるまで処理しない
	if (LogoDownCounter > 500)
	{
		//セレクト画像と矢印をnowNumの値に応じて描画
		DrawGraph(390 + abs(60 - (200 + (dirMoveCnt / 2 % 60)) % 30), dirNumY, im.ImageIdReturn("image/yazirushi2.png", SCENE_TITLE), true);
		//DrawGraph(500, 510, im.ImageIdReturn("image/Bar_Menu/Select.png", SCENE_TITLE), true);
		DrawGraph(500, 430, im.ImageIdReturn("image/Bar_Menu/Title.png", SCENE_TITLE), true);
		if (!clearFlag)	DrawGraph(510, 350, im.ImageIdReturn("image/Bar_Menu/Retry.png", SCENE_TITLE), true);
	}
	switch (nowNum) {
	case 0:
		dirNumY = clearFlag ? 440 : 360;
		break;
	case 1:
		dirNumY = 440;
		break;
	case 2:
		dirNumY = 520;
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

void ResultScene::DrawGameOverLogo(void)
{
	ImageMgr& im = ImageMgr::Instance();

	//Gの文字
	DrawGraph(10, min(LogoDownCounter,100), im.ImageIdReturn("image/Over/OVER/G.png", SCENE_TITLE), true);
	//Aの文字
	DrawGraph(110, min(LogoDownCounter - (DELAY_TIMER), 100), im.ImageIdReturn("image/Over/OVER/A.png", SCENE_TITLE), true);
	//Mの文字
	DrawGraph(210, min(LogoDownCounter - (DELAY_TIMER * 2), 100), im.ImageIdReturn("image/Over/OVER/M.png", SCENE_TITLE), true);
	//Eの文字
	DrawGraph(310, min(LogoDownCounter - (DELAY_TIMER * 3), 100), im.ImageIdReturn("image/Over/OVER/E.png", SCENE_TITLE), true);
	//Oの文字
	DrawGraph(410, min(LogoDownCounter - (DELAY_TIMER * 4), 100), im.ImageIdReturn("image/Over/OVER/O.png", SCENE_TITLE), true);
	//Vの文字
	DrawGraph(510, min(LogoDownCounter - (DELAY_TIMER * 5), 100), im.ImageIdReturn("image/Over/OVER/V.png", SCENE_TITLE), true);
	//Eの文字
	DrawGraph(610, min(LogoDownCounter - (DELAY_TIMER * 6), 100), im.ImageIdReturn("image/Over/OVER/E.png", SCENE_TITLE), true);
	//Rの文字
	DrawGraph(710, min(LogoDownCounter - (DELAY_TIMER * 7), 100), im.ImageIdReturn("image/Over/OVER/R.png", SCENE_TITLE), true);
}

void ResultScene::DrawGameClearLogo(void)
{
	ImageMgr& im = ImageMgr::Instance();

	//Cの文字
	DrawGraph(100, min(LogoDownCounter, 120), im.ImageIdReturn("image/Clear/CLEAR/C.png", SCENE_TITLE), true);
	//Lの文字
	DrawGraph(240, min(LogoDownCounter - (DELAY_TIMER * 2), 100), im.ImageIdReturn("image/Clear/CLEAR/L.png", SCENE_TITLE), true);
	//Eの文字
	DrawGraph(380, min(LogoDownCounter - (DELAY_TIMER * 3), 80), im.ImageIdReturn("image/Clear/CLEAR/E.png", SCENE_TITLE), true);
	//Aの文字
	DrawGraph(520, min(LogoDownCounter - (DELAY_TIMER * 4), 100), im.ImageIdReturn("image/Clear/CLEAR/A.png", SCENE_TITLE), true);
	//Rの文字
	DrawGraph(660, min(LogoDownCounter - (DELAY_TIMER * 5), 120), im.ImageIdReturn("image/Clear/CLEAR/R.png", SCENE_TITLE), true);
}

void ResultScene::DrawGoalTimer(void)
{
	ImageMgr& im = ImageMgr::Instance();
	numberImage = im.ImageIdReturn("image/UI/NewNum.png", SCENE_TITLE);
	//色変更
	SetDrawBright(255, 212, 0);

	second = _rtData.goalTime % 10;
	tenex = (_rtData.goalTime / 10) % 10;
	hunex = _rtData.goalTime / 100;
	DrawRectExtendGraph(400, 250, 450 + (NUM_X / 2), 350 + (NUM_Y / 2), NUM_X * second, 0, NUM_X, NUM_Y, numberImage, true);
	DrawRectExtendGraph(400 - (NUM_X / 2) * 2 - 20, 250, 450 + (NUM_X / 2) - (NUM_X / 2) * 2 - 20, 350 + (NUM_Y / 2), NUM_X * tenex, 0, NUM_X, NUM_Y, numberImage, true);
	DrawRectExtendGraph(400 - (NUM_X / 2) * 4 - 40, 250, 450 + (NUM_X / 2) - (NUM_X / 2) * 4 - 40, 350 + (NUM_Y / 2), NUM_X * hunex, 0, NUM_X, NUM_Y, numberImage, true);

	SetDrawBright(255, 255, 255);

	DrawGraph(450, 350, im.ImageIdReturn("image/Clear/秒.png", SCENE_TITLE), true);

}