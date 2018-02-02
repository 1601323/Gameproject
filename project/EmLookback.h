#pragma once
#include "Enemy.h"

#define EM_LOOKBACK_TIME 180

class HitClass;
class EnemyServer;
class Player;
class MapCtl;
class Rope;
class ModelMgr;

class EmLookback :
	public Enemy
{
private:
	HitClass& _hit;
	EnemyServer& _server;
	DIR _dir;			//ｷｬﾗの向き
	MapCtl* _map;
	ModelMgr* _modelmgr;

	Player& _player;
	Rope& _rope;
	int LookCount;		//振り返りのｶｳﾝﾄ
	int FearCount;		//怯みのｶｳﾝﾄ

	//void Draw();
	int circle;
	int loseSightCnt;	//PLが認識範囲から外れて見失うまでのカウント

	int modelhandle;
	int ETexture;
	int textureIndex;
	float modelDirAngle;

	ENEMY_STATE _state;
	ENEMY_DATA _emData;
	Circle _emEye;
	Position2 _pos;
	Position2 _tmpOffset;
	Position2 _initPos;
	EnemyServerData _individualData;	//エネミー本部に送るデータ

	float emSpeed; 
	float upAngle;
	float downAngle;
	bool returnFlag;
	float vx;
	float vy;
	void SetMove();						//動きについてセットする
	void setDir(void);					//向き
	void Visibility();					//視界判定
	void LookPl(void);					//プレイヤー発見時
	void LoseSight();
	void moveFear(void);				//怯み状態
	void EnemyFalter();					//怯み状態に遷移する
	void ReturnPoint();					//元の位置に戻っていく
	void returnDir(Position2 offset);	//向いている方向を返す
	void Gravity();
	void SetRange();
	void LimitMove();
public:
	EmLookback(Position2 pos,Player& pl,Rope& rope,EnemyServer& server,HitClass&  hit);
	~EmLookback();
	Rect _emRect;
	Rect& GetRect();
	ENEMY_TYPE _emType;
	ENEMY_TYPE& GetType();
	void SetInitPos();
	void Updata();

	ENEMY_STATE & GetState();

	void GetClass(HitClass* hit, Player& pl);

	void Draw(Position2 offset);

};

