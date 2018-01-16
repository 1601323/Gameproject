#pragma once

#include "Geometry.h"
#define SCREEN_SIZE_WIDTH  640
#define SCREEN_SIZE_HEIGHT 480

enum SCENE_TYPE {
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_CLEAR,		//���󉺓�͗��p���Ă��܂���
	SCENE_GAMEOVER,		//�d�l���m�莟��폜
	SCENE_MAX
};

class Scene;
class MapCtl;
class Rope;

class GameMain
{
private:
	Scene* _scene;
	RESULT_DATA _resultData;					//���U���g�f�[�^�ɂ���
	GameMain();
	GameMain(const GameMain&);
	//GameMain& operator=(const GameMain&);
	void ClearDataLoad();
	void ClearDataSave();

	void TransTitle();
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

	RESULT_DATA _newData[STAGE_MAX];						//�ŐV�̃N���A�f�[�^��ێ�
	RESULT_DATA _bestData[STAGE_MAX];						//�x�X�g�X�R�A��ێ�

	int nowStage;								//�I�΂ꂽ�X�e�[�W�ԍ���ێ�
	void Run();									//�Q�[�����s
	void ChangeScene(Scene* scene);				//�V�[���`�F���W

	void SetResultData(RESULT_DATA rt);			//���U���g�̃f�[�^��n��
	RESULT_DATA GetResultData();				//���U���g�f�[�^��Ԃ�
	void SetNowStage(int num);					//���݂̑I���X�e�[�W�ԍ������炤
	int GetNowStage();							//�I������Ă���X�e�[�W�ԍ���Ԃ�

	void checkBestScore();

	~GameMain();
};

