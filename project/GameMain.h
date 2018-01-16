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

	//�ް��ۑ����������ݗp�̊֐�
	void ClearDataLoad();
	void ClearDataSave();
<<<<<<< HEAD

	void TransTitle();							//�^�C�g���ւ̋����J��
=======
>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

<<<<<<< HEAD
	RESULT_DATA _newData[STAGE_MAX];			//�ŐV�̃N���A�f�[�^��ێ�
	RESULT_DATA _bestData[STAGE_MAX];			//�x�X�g�X�R�A��ێ�
	void BestDataSet();
	void NewDataSet();
	int nowStage;								//�I�΂ꂽ�X�e�[�W�ԍ���ێ�
=======
	RESULT_DATA _newData;						//�ŐV�̃N���A�f�[�^��ێ�
	RESULT_DATA _bestData;						//�x�X�g�X�R�A��ێ�

>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085
	void Run();									//�Q�[�����s
	void ChangeScene(Scene* scene);				//�V�[���`�F���W

	void SetResultData(RESULT_DATA rt);			//���U���g�̃f�[�^��n��
	RESULT_DATA GetResultData();				//���U���g�f�[�^��Ԃ�

	void checkBestScore();

	~GameMain();
};

