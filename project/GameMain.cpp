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
//�V�[���̗p��
//�Ƃ肠�����Q�[���̃V�[�������p�ӂ��Ă���
//�ǉ�������΂����ɐ��������Ă���
//�d�l�ύX�̉\������
void titleScene(SceneStruct* scene,Input* input);
void selectScene(SceneStruct* scene, Input* input);
void stageScene(SceneStruct* scene, Input* input);
void clearScene(SceneStruct* scene,Input* input);
void gameOverScene(SceneStruct* scene, Input* input);

void titleScene(SceneStruct* scene,Input* input)
{
	scene->func = titleScene;
//	DrawString(0,10,"�^�C�g���V�[��",GetColor(255,255,255));
}
void selectScene(SceneStruct* scene,Input* input)
{
	scene->func = selectScene;
	//DrawString(0,10,"�Z���N�g�V�[��",GetColor(255,255,255));
}
void stageScene(SceneStruct* scene, Input*input) 
{
	scene->func = stageScene;
//	DrawString(0, 10, "�Q�[���V�[��", GetColor(255, 255, 255));
}
void clearScene(SceneStruct* scene,Input* input)
{
	scene->func = clearScene;
//	DrawString(0,10,"�N���A�V�[��",GetColor(255,255,255));
}
void gameOverScene(SceneStruct* scene,Input* input)
{
	scene->func = gameOverScene;
//	DrawString(0,10,"�Q�[���I�[�o�[�V�[��",GetColor(255,255,255));
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
//�f�[�^�����[�h
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
//�f�[�^���Z�[�u
void GameMain::ClearDataSave()
{
	FILE* file;
	fopen_s(&file,"data/clearData.dat","wb");

	fread((char*)&_bestData,sizeof(_bestData),1,file);
	fclose(file);
}
//�ŐV�̃v���C�f�[�^��ۑ����܂�
void GameMain::NewDataSet()
{
	_newData[nowStage] = _resultData;
}
//�x�X�g�X�R�A���ް���ۑ����܂�
void GameMain::BestDataSet()
{
	if (_bestData[nowStage].goalFlag == false) {	//�����v���C����Ă��Ȃ��Ƃ�
		_bestData[nowStage] =_resultData;
	}
	//�^�C�����r
	else if (_bestData[nowStage].goalTime >= _newData[nowStage].goalTime) {
		_bestData[nowStage] = _resultData;
	}

}
//���U���g�Ɋւ��Ẵf�[�^���Z�b�g���܂�
void GameMain::SetResultData(RESULT_DATA rt)
{
	_resultData = rt;
}
//���U���g�Ɋւ��Ẵf�[�^��Ԃ��܂�
RESULT_DATA GameMain::GetResultData()
{
	return _resultData;
}
//���݂̃X�e�[�W���󂯎��܂�
void GameMain::SetNowStage(int num)
{
	nowStage = num;
}
//���݂̃X�e�[�W�ԍ���Ԃ��܂�
int GameMain::GetNowStage()
{
	return nowStage;
}
//̨��ް�ް����󂯎��܂�
void GameMain::SetFeverData(FEVER_DATA fd)
{
	_feverData = fd;
}
//̨��ް�ް���Ԃ��܂�
FEVER_DATA GameMain::ReturnFeverData()
{
	return _feverData;
}
//�^�C�g���ɋ����J�ڂ��s���܂�
void GameMain::TransTitle()
{
	if (CheckHitKey(KEY_INPUT_T) && CheckHitKey(KEY_INPUT_LCONTROL)) {
		ChangeScene(new TitleScene());
	}
}
//�Q�[���̎��s�̃��C������
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
	//�ŏ��ɑJ�ڂ���V�[����I�����Ă��܂��B
	//�ق��̃V�[���ɂ���Ƃ��͏㉺�ǂ�����ύX���Ă�������
	//���ݎg�p���ĂȂ��ł�
	SceneStruct scene = { titleScene };
	ChangeScene(new TitleScene());
	if (_scene != nullptr) {
		ImageMgr::Instance().ImageManager(_scene->GetScene());
	}
	else
	{
		//new�̐��؂�ւ��邱�Ƃł��ꂼ��m�F�ł��܂�
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