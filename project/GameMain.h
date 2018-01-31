#pragma once

#include "Geometry.h"

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
	FEVER_DATA _feverData;
	GameMain();
	GameMain(const GameMain&);
	//GameMain& operator=(const GameMain&);

	//�ް��ۑ����������ݗp�̊֐�
	void ClearDataLoad();
	void ClearDataSave();

	void TransTitle();							//�^�C�g���ւ̋����J��
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

	RESULT_DATA _newData[STAGE_MAX];			//�ŐV�̃N���A�f�[�^��ێ�
	RESULT_DATA _bestData[STAGE_MAX];			//�x�X�g�X�R�A��ێ�
	void BestDataSet();
	void NewDataSet();
	int nowStage;								//�I�΂ꂽ�X�e�[�W�ԍ���ێ�
	void SetNowStage(int num);
	int GetNowStage();
	void Run();									//�Q�[�����s
	void ChangeScene(Scene* scene);				//�V�[���`�F���W

	void SetResultData(RESULT_DATA rt);			//���U���g�̃f�[�^��n��
	RESULT_DATA GetResultData();				//���U���g�f�[�^��Ԃ�
	//����܂肫�ꂢ����Ȃ����ǂ����Ŏ������܂�
	void SetFeverData(FEVER_DATA fd);
	FEVER_DATA  ReturnFeverData();
	void checkBestScore();

	~GameMain();
};

