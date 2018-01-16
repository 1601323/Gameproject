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
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

	RESULT_DATA _newData;						//最新のクリアデータを保持
	RESULT_DATA _bestData;						//ベストスコアを保持

	void Run();									//ゲーム実行
	void ChangeScene(Scene* scene);				//シーンチェンジ

	void SetResultData(RESULT_DATA rt);			//リザルトのデータを渡す
	RESULT_DATA GetResultData();				//リザルトデータを返す

	void checkBestScore();

	~GameMain();
};

