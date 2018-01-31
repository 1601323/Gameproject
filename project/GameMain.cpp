#include <DxLib.h>
#include <stdio.h>
#include <iostream>
#include "Assert.h"
#include "GameMain.h"
#include "Scene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "Geometry.h"
#include "MapCtl.h"
#include "Input.h"
#include "Math.h"
#include "ImageMgr.h"
#include "ModelMgr.h"

using namespace std;

struct SceneStruct;
Input* input;


typedef void(*Function)(SceneStruct* scene, Input* input);

struct SceneStruct {
	Function func;

};
//シーンの用意
//とりあえずゲームのシーンだけ用意しておく
//追加があればここに随時足していく
//仕様変更の可能性あり
void titleScene(SceneStruct* scene,Input* input);
void selectScene(SceneStruct* scene, Input* input);
void stageScene(SceneStruct* scene, Input* input);
void clearScene(SceneStruct* scene,Input* input);
void gameOverScene(SceneStruct* scene, Input* input);

void titleScene(SceneStruct* scene,Input* input)
{
	scene->func = titleScene;
//	DrawString(0,10,"タイトルシーン",GetColor(255,255,255));
}
void selectScene(SceneStruct* scene,Input* input)
{
	scene->func = selectScene;
	//DrawString(0,10,"セレクトシーン",GetColor(255,255,255));
}
void stageScene(SceneStruct* scene, Input*input) 
{
	scene->func = stageScene;
//	DrawString(0, 10, "ゲームシーン", GetColor(255, 255, 255));
}
void clearScene(SceneStruct* scene,Input* input)
{
	scene->func = clearScene;
//	DrawString(0,10,"クリアシーン",GetColor(255,255,255));
}
void gameOverScene(SceneStruct* scene,Input* input)
{
	scene->func = gameOverScene;
//	DrawString(0,10,"ゲームオーバーシーン",GetColor(255,255,255));
}

GameMain::GameMain()
{
	FILE *fp;
	fopen_s(&fp, "error.txt", "w");
	if (fp == nullptr) {
		exit(1);
	}
	fclose(fp);

}

void GameMain::ChangeScene(Scene * scene) 
{
	if (_scene != nullptr) {
		delete _scene;
	}
	_scene = scene;
}
//データをロード
void GameMain::ClearDataLoad()
{
	FILE *file;
	fopen_s(&file, "data/clearData.dat", "rb");
	if (file == nullptr) {
		fopen_s(&file,"data/clearData.dat","wb");
		fwrite((char*)&_bestData,sizeof(_bestData),1,file);
	}
	else {
		fread((char*)&_bestData,sizeof(_bestData),1,file);
	}
	fclose(file);
}
//データをセーブ
void GameMain::ClearDataSave()
{
	FILE* file;
	fopen_s(&file,"data/clearData.dat","wb");

	fread((char*)&_bestData,sizeof(_bestData),1,file);
	fclose(file);
}
//最新のプレイデータを保存します
void GameMain::NewDataSet()
{
	_newData[nowStage] = _resultData;
}
//ベストスコアのﾃﾞｰﾀを保存します
void GameMain::BestDataSet()
{
	if (_bestData[nowStage].goalFlag == false) {	//一回もプレイされていないとき
		_bestData[nowStage] =_resultData;
	}
	//タイムを比較
	else if (_bestData[nowStage].goalTime >= _newData[nowStage].goalTime) {
		_bestData[nowStage] = _resultData;
	}

}
//リザルトに関してのデータをセットします
void GameMain::SetResultData(RESULT_DATA rt)
{
	_resultData = rt;
}
//リザルトに関してのデータを返します
RESULT_DATA GameMain::GetResultData()
{
	return _resultData;
}
//現在のステージを受け取ります
void GameMain::SetNowStage(int num)
{
	nowStage = num;
}
//現在のステージ番号を返します
int GameMain::GetNowStage()
{
	return nowStage;
}
//ﾌｨｰﾊﾞｰﾃﾞｰﾀを受け取ります
void GameMain::SetFeverData(FEVER_DATA fd)
{
	_feverData = fd;
}
//ﾌｨｰﾊﾞｰﾃﾞｰﾀを返します
FEVER_DATA GameMain::ReturnFeverData()
{
	return _feverData;
}
//タイトルに強制遷移を行います
void GameMain::TransTitle()
{
	if (CheckHitKey(KEY_INPUT_T) && CheckHitKey(KEY_INPUT_LCONTROL)) {
		ChangeScene(new TitleScene());
	}
}
//ゲームの実行のメイン部分
void GameMain::Run()
{
	DxLib::SetGraphMode(SCREEN_SIZE_X,SCREEN_SIZE_Y,32);
	ChangeWindowMode(true);
	//KeyState key;

	if (DxLib_Init() == -1) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	input = new Input();
	//最初に遷移するシーンを選択しています。
	//ほかのシーンにするときは上下どちらも変更してください
	//現在使用してないです
	SceneStruct scene = { titleScene };
	ChangeScene(new TitleScene());
	if (_scene != nullptr) {
		ImageMgr::Instance().ImageManager(_scene->GetScene());
	}
	else
	{
		//newの先を切り替えることでそれぞれ確認できます
		ChangeScene(new TitleScene());
	}
	ClearDataLoad();
	while (ProcessMessage() == 0) {

		ClearDrawScreen();
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
		TransTitle();
		input->Update();
		input->InputSet();
		//scene.func(&scene, input);
		_scene->Updata(input);
		ScreenFlip();
	}
	ClearDataSave();
	ImageMgr::Instance().ImageIdAllDelete();
	DxLib_End();
}
GameMain::~GameMain()
{
}