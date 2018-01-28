#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "ImageMgr.h"
#include "TitleScene.h"
#include "Input.h"
#include "SelectScene.h"


TitleScene::TitleScene()
{
	_updater = &TitleScene::FadeinTitle;
	SetMovieColorA8R8G8B8Flag(true);

	_menu = GAME_START;
	_minSensingValueL = SV_HIGH;
	operateFlag = false;
	_skyPos[0].x = -320;
	_skyPos[0].y = 0;
	_skyPos[1].x = 320;
	_skyPos[1].y = 0;
	skyImage = ImageMgr::Instance().ImageIdReturn("仮image/sky.png", SCENE_SELECT);
	uiMovie = "movie/titleMovie.avi";
	SeekMovieToGraph(ImageMgr::Instance().ImageIdReturn(uiMovie, SCENE_SELECT), 0);
	movieFlag = false;
	titleFlag = false;
	selectFlag = false;
	lightCnt = 0;
	flameCnt = 0;
	initFlag = false;
	///InitMovie();
}


TitleScene::~TitleScene()
{
	SeekMovieToGraph(ImageMgr::Instance().ImageIdReturn(uiMovie, SCENE_SELECT), 0);
}
void TitleScene::InitMovie()
{
	ImageMgr&im = ImageMgr::Instance();

	if (GetMovieStateToGraph(im.ImageIdReturn(uiMovie, SCENE_SELECT)) == 0) {
		PlayMovieToGraph(im.ImageIdReturn(uiMovie, SCENE_SELECT));
		PauseMovieToGraph(im.ImageIdReturn(uiMovie, SCENE_SELECT));
	}
}
void TitleScene::FadeinTitle(Input* input)
{
	if (initFlag == false) {
		InitMovie();
		initFlag = true;
	}

	lightCnt++;
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - (lightCnt * 2));
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (lightCnt >= 60) {
		_updater = &TitleScene::TitleMovie;
		lightCnt = 0;

	}
}
void TitleScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	if (selectFlag == true) {
		if (operateFlag == false) {
			MenuSelect(input);
		}
	}
	scroll();
	Draw();
#ifdef _DEBUG
	//DrawString(10, 0, "タイトル", GetColor(255, 255, 255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (selectFlag == true) {
			if (_menu == GAME_START) {
				gm.Instance().ChangeScene(new SelectScene());
				initFlag = false;
			}
			else if (_menu == GAME_EXPLAIN) {
				operateFlag = !operateFlag;
			}
		}
		else if (selectFlag == false) {
			selectFlag = true;
		}

	}
}
void  TitleScene::TitleMovie(Input* input)
{
	ImageMgr&im = ImageMgr::Instance();
	flameCnt++;
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	scroll();
	Draw();
	if (GetMovieStateToGraph(im.ImageIdReturn(uiMovie, SCENE_SELECT)) == 0) {
		PlayMovieToGraph(im.ImageIdReturn(uiMovie, SCENE_SELECT));
	}
	//DrawGraph(40, 50, im.ImageIdReturn(uiMovie,SCENE_SELECT), true);
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON
		|| flameCnt >= 180) {
		_updater = &TitleScene::NormalUpdata;
		titleFlag = true;
		selectFlag = true;
		flameCnt = 0;
	}
}
void TitleScene::scroll()
{
	for (int f = 0; f < 2; f++) {
		if (_skyPos[f].x <= -640) {
			_skyPos[f].x = 640;
		}
		_skyPos[f].x--;
	}
}
void TitleScene::MenuSelect(Input* input)
{
	if (inpInfo.num >= 1) {
		if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) {
			_menu = GAME_EXPLAIN;
			//if (_menu == GAME_START) {
			//	_menu =GAME_EXPLAIN;
			//}
		}
		else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
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
		if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
			_menu = GAME_EXPLAIN;
			//if (_menu == GAME_START) {
			//	_menu =GAME_EXPLAIN;
			//}
		}
		else if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
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
	ImageMgr& im = ImageMgr::Instance();
	for (int f = 0; f < 2; f++) {
		DrawGraph(_skyPos[f].x, _skyPos[f].y, skyImage, true);
	}

	DrawGraph(0, 0, im.ImageIdReturn("仮image/titleImage.png", SCENE_SELECT), true);


	if (selectFlag == true) {
		//DrawBox(200, 500, 300, 600, 0xffaaff, true);
		//if (_menu == GAME_START) {
		//	DrawBox(200, 500, 300, 600, 0xffffff, true);
		//}
		//DrawString(250, 550, "Game", 0xffff99);
		DrawGraph(150, 350, im.ImageIdReturn("仮image/title/select_Game.png", SCENE_SELECT), true);

		//DrawBox(400, 500, 500, 600, 0xffaaff, true);
		//if (_menu == GAME_EXPLAIN) {
		//	DrawBox(400, 500, 500, 600, 0xffffff, true);
		//}
		//DrawString(450, 550, "explain", 0xffff99);
		DrawGraph(150, 500, im.ImageIdReturn("仮image/title/Pause_ope.png", SCENE_SELECT), true);
		if (_menu == GAME_START) {
			DrawGraph(70, 350, im.ImageIdReturn("仮image/UI/dirset1.png", SCENE_SELECT), true);
		}
		else if (_menu == GAME_EXPLAIN) {
			DrawGraph(70, 500, im.ImageIdReturn("仮image/UI/dirset1.png", SCENE_SELECT), true);
		}
	}
	if (operateFlag == true) {
		DrawBox(200, 400, 500, 500, 0xffffff, true);
		DrawString(220, 410, "未実装", 0x000000);
		DrawString(250, 450, "もう一度エンターで選択を解除", 0x000000);
	}

	DrawGraph(400, -105, im.ImageIdReturn(uiMovie, SCENE_SELECT), true);
	DrawGraph(40, 50, im.ImageIdReturn("仮image/title.png", SCENE_SELECT), true);

}
