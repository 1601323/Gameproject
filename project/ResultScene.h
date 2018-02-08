#pragma once
#include "Scene.h"

class Input;
class ModelMgr;

enum NEXT_MODE {
	JUMP_RETRY,
	JUMP_TITLE,
	JUMP_MAX
};

class ResultScene :
	public Scene
{
private:
	RESULT_DATA _rtData;
	void(ResultScene::*_updater)(Input* input);
	KEY key;
	KEY lastKey;
	INPUT_INFO inpInfo;
	NEXT_MODE _mode;
	SENSING_VALUE _minSensingValueL;
	NEXT_MODE mode[JUMP_MAX];
	int nowNum;
	//�\���p�֐�
	int numberImage;
	int second;
	int tenex;
	int hunex;

	int dirNumY;
	bool clearFlag;
	bool selectFlag;

	ModelMgr* _modelmgr;
	int playerModelHandle;
	int playerModelWithFlask;
	int textureIndex;
	int textureIndexFlask;
	int dirMoveCnt;                 //���̈ړ��J�E���g 
	int AnnouncCnt;                 //�^�C���\���p�̃A�i�E���X�J�E���g
	int medicineHandle;
	int LogoDownCounter;            //�Q�[���I�[�o�[�̕����������Ă���悤�̃J�E���^�[
	bool BgmFlag;					//BGM��炷�t���O
	std::string tmpName;
	SCENE_TYPE tmpScene;

	float AnimTotalTimeH;           //�A�j���[�V���������� H��happy��H
	float AnimNowTimeH;             //���̃A�j���[�V�������� H��happy��H
	int  AnimIndexH;                //�A�j���[�V�����C���f�b�N�X H��happy��H

	float AnimTotalTimeS;           //�A�j���[�V���������� S��happy��S
	float AnimNowTimeS;             //���̃A�j���[�V�������� S��happy��S
	int  AnimIndexS;                //�A�j���[�V�����C���f�b�N�X S��happy��S

	void NormalUpdata(Input* input);

	void GameClear();
	void GameOver();
	void Select(Input* input);
	void Draw();
	void DrawGameOverLogo(void);
	void DrawGameClearLogo(void);
	void DrawGoalTimer(void);
public:
	ResultScene();
	~ResultScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};