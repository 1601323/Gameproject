#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Input.h"

#define ACCEL_X (0.3f)					//加速度
#define GRABITY (0.9f)					//重力
#define MAX_SPEED (3.0f)				//ｽﾋﾟｰﾄﾞのMAX値
#define MAX_GRABITY (15.0f)				//重力のMAX値
#define WALL_SPEED (1.0f)				//壁登りのｽﾋﾟｰﾄﾞ
#define JUMP_POWER (15.0f)				//ｼﾞｬﾝﾌﾟ力
#define VANISH_CNT (3)					//消えるまでのｶｳﾝﾄ

class Input;
class HitClass;
class Camera;
class MapCtl;
class Rope;
class GameScene;

struct L_STICK;
struct R_STICK;

enum SENSING_VALUE;

class Player :public Object
{
private:
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	HitClass *_hit;
	MapCtl *_map;
	Rope* _rope;
	//GameScene& gmScen;
	CHAR_ST _state;						//ｷｬﾗの状態
	Position2 _pos;						//ｷｬﾗの座標
	DIR _dir;							//ｷｬﾗの向き
	Position2 initPos;					//初期位置保存
	float vx;							//x速度
	float vy;							//y速度
	int vanCnt;							//消えるまでのｶｳﾝﾄ
	bool JumpFlag;						//ｼﾞｬﾝﾌﾟのﾌﾗｸﾞ
	bool WallFlag;						//壁に張り付くフラグ
	bool moveFlag;						//壁に張り付いているとき動けるかのフラグ
	bool moveRopeJumpFlag;				//ﾛｰﾌﾟｼﾞｬﾝﾌﾟ処理
	void HitToEnemy();					//敵と当たった時
	char keyData[256];
	char oldkeyData[256];

	void setMove(Input* input);			//移動関数
	void setState(void);				//ｽﾃｰﾀｽ制御
	void setDir(Input* input);			//向き制御
	void gravity(void);					//重力
	bool accelePL(void);				//移動処理
	bool moveJump(void);				//ｼﾞｬﾝﾌﾟ処理
	bool moveWall(void);				//壁移動処理
	bool moveRope(void);				//ﾛｰﾌﾟ状態
	bool stVanish(void);				//ｽﾃﾙｽ処理
	bool stFever(void);					//ﾌｨｰﾊﾞｰ処理

public:
	Player();
	~Player();
	Rect _plRect;
	Rect& GetRect();
	void Update(Input* input);
	void Draw(Position2& offset);

	CHAR_ST GetcharState(void);			//ｽﾃｰﾀｽ取得
	Position2& GetPos(void);			//pos取得
	void SetInitPos();					//初期位置を返す
	DIR GetDir(void);					//dir取得	
	void SetRetryPos(Position2 midPos);
	void Getclass(HitClass* h, Rope*r);	//hitクラスを受け取るための関数

	bool EnterDoor();					//仮実装　ドアに入ったらtrueにします
	float playerSpeedTable[SV_MAX] = { 0.f,1.f,MAX_SPEED,MAX_SPEED };//スティックの傾き応じたplayerのspeedの上限テーブル
	SENSING_VALUE _minSensingValueL;  // ｽﾃｨｯｸの入力を感知する最低の値 L
};

