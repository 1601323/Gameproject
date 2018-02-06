#pragma once
#include "Scene.h"

#define COLOR_NUM (40)

class Input;
class ModelMgr;

enum SENSING_VALUE;
class SelectScene :
	public Scene
{
private:
	void(SelectScene::*_updater)(Input* input);
	void NormalUpdata(Input* input);
	void Draw();
	void Select(Input* input);
	ModelMgr* _modelmgr;

	KEY key;
	KEY lastKey;
	INPUT_INFO _inpInfo;
	SENSING_VALUE _minSensingValueL;

	int nowNum;
	int mapNumber[STAGE_MAX];
	int SelectMap;
	int modelhandle;        //�v���C���[���f���n���h��
	int colorNum;           //�J���[�ύX�^�C�}�[
	int  AnimIndex;         //�A�j���[�V������
	float AnimTotalTime;    //�e�A�j���[�V�����̃g�[�^���^�C��
	float AnimNowTime;      //�e�A�j���[�V�����̌��݃^�C��
	bool selectFlag;		//�Z���N�g�̃L�[�̐���t���O

	int w;
	int h;
	char* stageNum[3];
public:
	SelectScene();
	~SelectScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

