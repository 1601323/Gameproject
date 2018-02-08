#pragma once
#include "Gimmick.h"
#include "Geometry.h"
#include "Input.h"
class Input;
class Rope;
//class Player;
class MapCtl;
class ModelMgr;

class GimPull :
	public Gimmick
{
private:
	HitClass* _hit;
	Rope& _rope;
	Player& _player;
	GimmickState _state;
	Position2 _pos;
	MapCtl* _map;
	ModelMgr* _modelmgr;
	Position2 _initPos;
	//確認用
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	int count;				//一定量動く範囲を設定する
	int modelhandle;
	int textureIndex;       //テクスチャのindex

	void CheckDoMove();		//動作条件判定
	void Move();			//ギミックの動作について
	void SetInit();
public:
	GimPull(Position2 pos,Rope& r,Player& p);
	~GimPull();
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	void Updata(Input& _input);
	void Draw(Position2 offset);			//描画
	void ChangeStageTexture();
};

