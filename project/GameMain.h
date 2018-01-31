#pragma once

#include "Geometry.h"

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
	FEVER_DATA _feverData;
	GameMain();
	GameMain(const GameMain&);
	//GameMain& operator=(const GameMain&);

	//ﾃﾞｰﾀ保存＆書き込み用の関数
	void ClearDataLoad();
	void ClearDataSave();

	void TransTitle();							//タイトルへの強制遷移
public:
	static GameMain& Instance() {
		static GameMain instance;
		return instance;
	}

	RESULT_DATA _newData[STAGE_MAX];			//最新のクリアデータを保持
	RESULT_DATA _bestData[STAGE_MAX];			//ベストスコアを保持
	void BestDataSet();
	void NewDataSet();
	int nowStage;								//選ばれたステージ番号を保持
	void SetNowStage(int num);
	int GetNowStage();
	void Run();									//ゲーム実行
	void ChangeScene(Scene* scene);				//シーンチェンジ

	void SetResultData(RESULT_DATA rt);			//リザルトのデータを渡す
	RESULT_DATA GetResultData();				//リザルトデータを返す
	//あんまりきれいじゃないけどここで持たせます
	void SetFeverData(FEVER_DATA fd);
	FEVER_DATA  ReturnFeverData();
	void checkBestScore();

	~GameMain();
};

