#include <DxLib.h>
#include "GameMain.h"
#include "ImageMgr.h"
#include "TitleScene.h"
#include "MovieScene.h"



MovieScene::MovieScene()
{
	_updater = &MovieScene::NormalUpdata;
	SetMovieColorA8R8G8B8Flag(false);
	ImageMgr& im = ImageMgr::Instance();

	waitMovie = "movie/kari1.ogv";
	DxLib::SeekMovieToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE), 120);

	playFlag = false;

}


MovieScene::~MovieScene()
{
}
void MovieScene::NormalUpdata(Input* input) 
{
	ImageMgr& im = ImageMgr::Instance();
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	DrawString(0,0,"uwaaaaaaaaaaaa",0xffffff);
	DrawExtendGraph(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, im.ImageIdReturn("movie/kari1.ogv", SCENE_TITLE), true);

	if (GetMovieStateToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE)) == 0) {
		PlayMovieToGraph(im.ImageIdReturn(waitMovie,SCENE_TITLE));
	}
	DrawExtendGraph(0,0,SCREEN_SIZE_X,SCREEN_SIZE_Y,im.ImageIdReturn(waitMovie,SCENE_TITLE),true);
	if (GetJoypadInputState(DX_INPUT_KEY_PAD1)|| CheckHitKeyAll()
		|| GetMovieStateToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE)) == 0) {
		gm.Instance().ChangeScene(new TitleScene());
	}
}
SCENE_TYPE MovieScene::GetScene()
{
	return SCENE_MOVIE;
}
void MovieScene::Updata(Input* input)
{
	(this->*_updater)(input);
}
void MovieScene::playMovie()
{
	ImageMgr&im = ImageMgr::Instance();
	if (playFlag == false)
	{
		if (DxLib::GetMovieStateToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE)) == 0)
		{
			DxLib::PlayMovieToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE));
			playFlag = true;
		}
	}
	else if (playFlag == true)
	{
		if (DxLib::GetMovieStateToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE)) == 0)
		{
			DxLib::SeekMovieToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE), 0);
			DxLib::PlayMovieToGraph(im.ImageIdReturn(waitMovie, SCENE_TITLE));
		}
	}
	DxLib::DrawExtendGraph(630, 390, 1030, 760, im.ImageIdReturn(waitMovie, SCENE_TITLE), true);

}