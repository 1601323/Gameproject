#pragma once
#include <memory>
#include <list>
#include <map>
#include "Gimmick.h"
#include "Geometry.h"

class HitClass;
class Player;
class ModelMgr;

class SensorDoor :
	public Gimmick
{
private:
	HitClass* _hit;
	Player& _pl;
	ModelMgr* _modelmgr;
	GimmickState  _state;
	Position2 _pos;
	Position2 _initPos;
	//動作確認用
	bool keyFlag;
	bool openFlag;

	int count;			//ドア開閉のためのカウント
	float doorCountOpen;		//ドア開の調整カウント
	float doorCountClose;		//ドア開の調整カウント


	int modelhandle;
	int AttachIndex;
	float totalTime;

	int AttachIndexClose;
	float totalTimeClose;

	void CheckDoMove();	//動作条件判定
	void CheckHit();	//あたり判定をプレイヤーの場所によって変えるので書いておく
	void Draw(Position2 offset);		//描画
	void SetInit();
public:
	SensorDoor(Position2 pos,Player& p);
	~SensorDoor();
	bool enterFlag;
	void Updata();
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	RESULT_DATA& GetClearData();
};

