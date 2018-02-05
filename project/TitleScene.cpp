#include <DxLib.h>
#include <iostream>
#include "GameMain.h"
#include "ImageMgr.h"
#include "TitleScene.h"
#include "Input.h"
#include "SelectScene.h"
#include "MovieScene.h"

using namespace std;
TitleScene::TitleScene()
{
	_updater = &TitleScene::FadeinTitle;
	SetMovieColorA8R8G8B8Flag(true);

	_menu = GAME_START;
	_minSensingValueL = SV_HIGH;
	operateFlag = false;
	_skyPos[0].x = -512;
	_skyPos[0].y = 0;
	_skyPos[1].x = 512;
	_skyPos[1].y = 0;
	skyImage = ImageMgr::Instance().ImageIdReturn("image/sky.png", SCENE_SELECT);
	uiMovie = "movie/titleMovie.avi";
	//SeekMovieToGraph(ImageMgr::Instance().ImageIdReturn(uiMovie, SCENE_SELECT), 0);
	movieFlag = false;
	titleFlag = false;
	selectFlag = false;
	lightCnt = 0;
	flameCnt = 0;
	dirMoveCnt = 0;
	menuCnt = 640;
	initFlag = false;
	padFlag = false;
	noInputTime = 0;
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
	CheckPad();
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
	noInputTime++;
#ifdef _DEBUG
	//DrawString(10, 0, "タイトル", GetColor(255, 255, 255));
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (selectFlag == true) {
			if (_menu == GAME_START) {
				//gm.Instance().ChangeScene(new SelectScene());
				_updater = &TitleScene::FadeoutTitle;
				noInputTime = 0;
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
	if (GetJoypadInputState(DX_INPUT_KEY_PAD1) || CheckHitKeyAll()) {
		noInputTime = 0;
	}
	//とりあえず一分放置で遷移
	if (noInputTime >= 3600) {
		gm.Instance().ChangeScene(new MovieScene());
		noInputTime = 0;
	}
	//操作説明をしたから出したい
	if (operateFlag == false) {
		if (menuCnt < 640) {
			menuCnt += 10;
		}
	}
	else if (operateFlag == true) {
		if (menuCnt > 0) {
			menuCnt -= 10;
		}
	}
}
void TitleScene::FadeoutTitle(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	Draw();
	lightCnt++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 + lightCnt);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (lightCnt >= 120) {
		gm.Instance().ChangeScene(new SelectScene());
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
		if (_skyPos[f].x <= -1024) {
			_skyPos[f].x = 1024;
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
void TitleScene::CheckPad()
{
	padFlag = inpInfo.num >0 ? true : false;
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

	DrawGraph(0, 0, im.ImageIdReturn("image/title/TitleImage.png", SCENE_SELECT), true);

	if (selectFlag == true) {
		//DrawBox(200, 500, 300, 600, 0xffaaff, true);
		//if (_menu == GAME_START) {
		//	DrawBox(200, 500, 300, 600, 0xffffff, true);
		//}
		//DrawString(250, 550, "Game", 0xffff99);

		DrawGraph(70,410,im.ImageIdReturn("image/title/TitleBoard.png",SCENE_SELECT),true);

		DrawGraph(250, 450, im.ImageIdReturn("image/title/TitleSelectSpell.png", SCENE_SELECT), true);

		//DrawBox(400, 500, 500, 600, 0xffaaff, true);
		//if (_menu == GAME_EXPLAIN) {
		//	DrawBox(400, 500, 500, 600, 0xffffff, true);
		//}
		//DrawString(450, 550, "explain", 0xffff99);
		DrawGraph(250, 530, im.ImageIdReturn("image/title/TitleSousaSpell.png", SCENE_SELECT), true);
		if (_menu == GAME_START) {
			dirMoveCnt += 2;
			DrawGraph(170 - abs(30 - ( 200 + (dirMoveCnt / 2 % 60)) % 59), 445, im.ImageIdReturn("image/yazirushi.png", SCENE_SELECT), true);
		}
		else if (_menu == GAME_EXPLAIN) {
			dirMoveCnt += 2;
			DrawGraph(170 - abs(30 - (200 + (dirMoveCnt / 2 % 60)) % 59), 525, im.ImageIdReturn("image/yazirushi.png", SCENE_SELECT), true);
		}
	}

	DrawGraph(400, -105, im.ImageIdReturn(uiMovie, SCENE_SELECT), true);
	DrawExtendGraph(10, 10,750,450, im.ImageIdReturn("image/title.png", SCENE_SELECT), true);
	//if (padFlag == false) {
	//	DrawGraph(0, max(0, menuCnt), im.ImageIdReturn("image/Operation/Operation_key.png", SCENE_SELECT), true);
	//}
	 if (inpInfo.num >=1) {
		DrawGraph(0, max(0, menuCnt), im.ImageIdReturn("image/Operation/Operation_pad.png", SCENE_SELECT), true);
	}else{
		 DrawGraph(0, max(0, menuCnt), im.ImageIdReturn("image/Operation/Operation_key.png", SCENE_SELECT), true);
	 }

}
