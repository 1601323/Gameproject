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

	//ﾃﾞｰﾀ保存＆書き込み用の関数
	void ClearDataLoad();
	void ClearDataSave();
<<<<<<< HEAD

	void TransTitle();							//タイトルへの強制遷移
=======
>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

<<<<<<< HEAD
	RESULT_DATA _newData[STAGE_MAX];			//最新のクリアデータを保持
	RESULT_DATA _bestData[STAGE_MAX];			//ベストスコアを保持
	void BestDataSet();
	void NewDataSet();
	int nowStage;								//選ばれたステージ番号を保持
=======
	RESULT_DATA _newData;						//最新のクリアデータを保持
	RESULT_DATA _bestData;						//ベストスコアを保持

>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085
	void Run();									//ゲーム実行
	void ChangeScene(Scene* scene);				//シーンチェンジ

	void SetResultData(RESULT_DATA rt);			//リザルトのデータを渡す
	RESULT_DATA GetResultData();				//リザルトデータを返す

	void checkBestScore();

	~GameMain();
};

