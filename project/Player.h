#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Input.h"

#define ACCEL_X (0.3f)					//加速度
#define MAX_SPEED (3.0f)				//ｽﾋﾟｰﾄﾞのMAX値
#define MAX_GRAVITY (15.0f)				//重力のMAX値
#define WALL_SPEED (1.0f)				//壁登りのｽﾋﾟｰﾄﾞ
#define JUMP_POWER (15.0f)				//ｼﾞｬﾝﾌﾟ力
#define VANISH_CNT (3)					//消えるまでのｶｳﾝﾄ
#define FEVER_CNT (10)					//フィーバーの時間
#define ANIMATION_SPEED_SLOW (0.3f)     //アニメーションの速さ(超低速)
#define ANIMATION_SPEED_DEF (0.5f)      //アニメーションの速さ(低速)
#define ANIMATION_SPEED_HIGH (0.8f)     //アニメーションの速さ(普通)

class Input;
class HitClass;
class Camera;
class MapCtl;
class Rope;
class GameScene;
class ModelMgr;

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
	Position3 _pos;						//ｷｬﾗの座標
	DIR _dir;							//ｷｬﾗの向き
	Position3 initPos;					//初期位置保存
	float vx;							//x速度
	float vy;							//y速度
	int vanCnt;							//消えるまでのｶｳﾝﾄ
	bool feverFlag;						//フィーバーフラグ
	int feverTime;						//フィーバーの時間
	bool JumpFlag;						//ｼﾞｬﾝﾌﾟのﾌﾗｸﾞ
	bool WallFlag;						//壁に張り付くフラグ
	bool moveFlag;						//壁に張り付いているとき動けるかのフラグ
	bool moveRopeJumpFlag;				//ﾛｰﾌﾟｼﾞｬﾝﾌﾟ処理
	bool fMoveRight;					//ﾌｨｰﾊﾞｰ時、壁のぼりを制御するフラグ
	bool fMoveLeft;						//同上
	bool deathFlag;						//まさに死亡フラグ（_stateでは管理しきれないみたいなので
	bool helpFever;						//ﾌｨｰﾊﾞｰ終了時の補正のために用意
	void HitToEnemy();					//敵と当たった時
	char keyData[256];
	char oldkeyData[256];

	void setMove(Input* input);			//移動関数
	void setState(void);				//ｽﾃｰﾀｽ制御
	void setDir(Input* input);			//向き制御
	void gravity(void);					//重力
	bool accelePL(void);				//移動処理
	void InputSetMove();				//移動処理の中の入力を受け持ちます
	bool moveJump(void);				//ｼﾞｬﾝﾌﾟ処理
	bool moveWall(void);				//壁移動処理
	bool moveRope(void);				//ﾛｰﾌﾟ状態
	void moveFever();
	bool stVanish(void);				//ｽﾃﾙｽ処理
	bool stFever(void);					//ﾌｨｰﾊﾞｰ処理
	bool plPlaceCheck();				//ﾌｨｰﾊﾞｰ終了時、ﾌﾟﾚｲﾔｰが壁の中にいないかチェックします

	void FeverUpdata(Input* input);		//フィーバー時に呼び出す全体の処理
	void FeverJump();					//フィーバー用のジャンプ処理
	void FeverGravity();				//フィーバー用の重力処理
	void FeverWall();					//フィーバー用の壁移動処理

	void AnimationSwitching(void);      //アニメーション切り替えよう関数
	Position2 modelPlayerPos;

	Position2 tmpOffset;
	//いきなりなのでここに書いてます
	ModelMgr* _modelmgr;
	int modelhandle;
	int alfa;
	int tranceMax;
	float  AnimTotalTime[ACTION_MAX];
	float  AnimNowTime[ACTION_MAX];
	float modelDirAngle;
	int  AnimIndex[ACTION_MAX];
	float outlineNum;
	Position3 TempMoveVector;

public:
	Player();
	~Player();
	Rect _plRect;
	Rect& GetRect();
	void Update(Input* input);
	void Draw(Position2& offset);

	CHAR_ST GetcharState(void);			//ｽﾃｰﾀｽ取得
	Position2& GetPos(void);			//pos取得
	void SetInitPos();					//初期位置に戻す
	void SetInitPos(Position2 p);		//初期位置をセットする
	DIR GetDir(void);					//dir取得	
	void SetRetryPos(Position2 midPos);
	void Getclass(HitClass* h, Rope*r);	//hitクラスを受け取るための関数

	bool EnterDoor();					//仮実装　ドアに入ったらtrueにします
	float playerSpeedTable[SV_MAX] = { 0.f,1.f,MAX_SPEED,MAX_SPEED };//スティックの傾き応じたplayerのspeedの上限テーブル
	SENSING_VALUE _minSensingValueL;  // ｽﾃｨｯｸの入力を感知する最低の値 L

	Position2& GetModelPos(void);
};

