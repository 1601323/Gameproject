#pragma once

#include "Geometry.h"
#define SCREEN_SIZE_WIDTH  640
#define SCREEN_SIZE_HEIGHT 480

enum SCENE_TYPE {
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_CLEAR,		//現状下二つは利用していません
	SCENE_GAMEOVER,		//仕様が確定次第削除
	SCENE_MAX
};

class Scene;
class MapCtl;
class Rope;

class GameMain
{
private:
	Scene* _scene;
	RESULT_DATA _resultData;					//リザルトデータについて
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

	RESULT_DATA _newData[STAGE_MAX];						//最新のクリアデータを保持
	RESULT_DATA _bestData[STAGE_MAX];						//ベストスコアを保持

	int nowStage;								//選ばれたステージ番号を保持
	void Run();									//ゲーム実行
	void ChangeScene(Scene* scene);				//シーンチェンジ

	void SetResultData(RESULT_DATA rt);			//リザルトのデータを渡す
	RESULT_DATA GetResultData();				//リザルトデータを返す
	void SetNowStage(int num);					//現在の選択ステージ番号をもらう
	int GetNowStage();							//選択されているステージ番号を返す

	void checkBestScore();

	~GameMain();
};

