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


ResultScene::ResultScene()
{
	_updater = &ResultScene::NormalUpdata;
	_minSensingValueL = SV_HIGH;
	_mode = JUMP_SELECT;
	clearFlag = false;
	selectFlag = false;
	nowNum = 0;
	_modelmgr = ModelMgr::Instance();
	//モデル読み込み
	playerModelHandle = MV1LoadModel("player_model/player.pmx");
	medicineHandle = MV1LoadModel("gimmick_model/フラスコ/丸底フラスコ.pmx");
	//喜びのテクスチャの読み込み
	smileTexture = LoadGraph("player_model/face2.png");
	//悲しんでいるときのテクスチャ
	sadTexture = LoadGraph("");
	//アニメーションをアタッチ+総時間の設定
	AnimIndexH = MV1AttachAnim(playerModelHandle, ACTION_HAPPY, -1, false);
	AnimTotalTimeH = MV1GetAttachAnimTotalTime(playerModelHandle, AnimIndexH);

	AnimIndexS = MV1AttachAnim(playerModelHandle, ACTION_AOONI, -1, false);
	AnimTotalTimeS = MV1GetAttachAnimTotalTime(playerModelHandle, AnimIndexS);

	for (int i = 0; i < 2; i++)
	{
		//顔のテクスチャのindexを取得
		textureIndex[i] = MV1GetMaterialDifMapTexture(playerModelHandle, i+1);
	}
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
	//DrawString(10, 10, "リザルトに遷移してるよ！", 0xffffff);
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (nowNum == JUMP_SELECT) {
			gm.Instance().ChangeScene(new SelectScene());
		}
		else if (nowNum == JUMP_TITLE) {
			gm.Instance().ChangeScene(new TitleScene());
		}
		else if (nowNum == JUMP_RETRY) {
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
		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX - 1;
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
	ImageMgr& im = ImageMgr::Instance();
	if (clearFlag == true) {
		//背景
		DrawGraph(0, 0, im.ImageIdReturn("仮image/result の仮です/Result.png", SCENE_TITLE), true);
		DrawGraph(0, 0, im.ImageIdReturn("仮image/result の仮です/Result2.png", SCENE_TITLE), true);
		DrawGraph(150, 10, im.ImageIdReturn("仮image/UI/clear.png", SCENE_TITLE), true);

		//プレイヤー happy
		//アニメーションのフレームを進める
		AnimNowTimeH += 0.5f;
		//現在のアニメーションが最大フレームまでいったらループする
		if (AnimNowTimeH >= AnimTotalTimeH)
		{
			AnimNowTimeH = 0;
		}
		//モデルの回転角度の設定(ラジアン)
		MV1SetRotationXYZ(playerModelHandle, VGet(0.f, AngleRad(45.f), 0.f));
		//アニメーションをアタッチ
		MV1SetAttachAnimTime(playerModelHandle, AnimIndexH, AnimNowTimeH);
		//モデルのposを設定+ワールド座標からスクリーンへ変換
		MV1SetPosition(playerModelHandle, ConvWorldPosToScreenPos(VGet(600.f, 600, 0.f)));
		//モデルの拡大縮小値の設定
		MV1SetScale(playerModelHandle, VGet(4.0f, 4.0f, 4.0f));
		//顔のテクスチャを笑顔の方に変更
		MV1SetTextureGraphHandle(playerModelHandle, textureIndex[0], smileTexture, FALSE);
		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(playerModelHandle, 0.0f);

		//薬
		//モデルのposを設定+ワールド座標からスクリーンへ変換
		//MV1SetPosition(medicineHandle, ConvWorldPosToScreenPos(VGet(500.f, 600, 0.f)));
		//モデルの拡大縮小値の設定
		//MV1SetScale(medicineHandle, VGet(15.0f, 15.0f, 15.0f));
		//モデルを輪郭線0.0fで描画 
		//_modelmgr->Draw(medicineHandle, 0.0f);

		//スコアタイムの画像読み込みと表示
		numberImage = im.ImageIdReturn("仮image/UI/NewNum.png", SCENE_RESULT);
		second = _rtData.goalTime % 10;
		tenex = (_rtData.goalTime / 10) % 10;
		hunex = _rtData.goalTime / 100;

		DrawRectExtendGraph(400, 150, 400 + (NUM_X / 2), 200 + (NUM_Y / 2), NUM_X * second, 0, NUM_X, NUM_Y, numberImage, true);
		DrawRectExtendGraph(400 - (NUM_X / 2) * 1, 150, 400 + (NUM_X / 2) - (NUM_X / 2), 200 + (NUM_Y / 2), NUM_X * tenex, 0, NUM_X, NUM_Y, numberImage, true);
		DrawRectExtendGraph(400 - (NUM_X / 2) * 2, 150, 400 + (NUM_X / 2) - (NUM_X / 2) * 2, 200 + (NUM_Y / 2), NUM_X * hunex, 0, NUM_X, NUM_Y, numberImage, true);

		//DrawString(300, 300, "セレクト", 0xffffff);
		//DrawString(300, 320, "タイトル", 0xffffff);
	}
	else if (clearFlag == false) {
		//確認用でGAMEOVERにも書いた-----------------------------------------------------
		//背景
		DrawGraph(0, 0, im.ImageIdReturn("仮image/Over/GameOver.png", SCENE_TITLE), true);
		//DrawGraph(20, dirNumY, im.ImageIdReturn("仮image/UI/dirset1.png", SCENE_TITLE), true);
		//DrawGraph(150, 10, im.ImageIdReturn("仮image/UI/clear.png", SCENE_TITLE), true);

		//プレイヤー
		AnimNowTimeS += 0.5f;
		if (AnimNowTimeS >= AnimTotalTimeS)
		{
			AnimNowTimeS = 0;
		}
		MV1SetRotationXYZ(playerModelHandle, VGet(0.f,0.f, 0.f));
		MV1SetAttachAnimTime(playerModelHandle, AnimIndexS, AnimNowTimeS);
		MV1SetPosition(playerModelHandle, ConvWorldPosToScreenPos(VGet(400.f, 580, 0.f)));
		MV1SetScale(playerModelHandle, VGet(5.0f, 5.0f, 5.0f));
		//MV1SetTextureGraphHandle(playerModelHandle, textureIndex[1], smileTexture, FALSE);

		//モデルを輪郭線0.0fで描画 
		_modelmgr->Draw(playerModelHandle, 0.0f);

		DrawGraph(0, 0, im.ImageIdReturn("仮image/Over/Fence.png", SCENE_TITLE), true);


		//DrawString(300, 280, "リトライ", 0xffffff);
		//DrawString(300, 300, "セレクト", 0xffffff);
		//DrawString(300, 320, "タイトル", 0xffffff);
	}
	switch (nowNum) {
	case 0:
		//DrawString(280, 280, "→", 0xffffff);
		//DrawGraph(20, clearFlag ? 250 : 200, im.ImageIdReturn("仮image/UI/dirset1.png", SCENE_TITLE), true);

		//dirNumY = clearFlag ? 250 : 200;
		break;
	case 1:
		//DrawString(280, 300, "→", 0xffffff);
		//DrawGraph(20, 250, im.ImageIdReturn("仮image/UI/dirset1.png", SCENE_TITLE), true);

		//dirNumY = 250;
		break;
	case 2:
		//DrawString(280, 320, "→", 0xffffff);

		dirNumY = 360;
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