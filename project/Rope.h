#pragma once
#include <list>
#include "Object.h"
#include "Input.h"

#define ROPE_SPEED (6.5f)          //移動スピード
#define ROPE_THETA (45.f)        //ロープの斜めの角度右向きver
#define ROPE_THETA2 (135.f)      //ロープの斜めの角度左向きver
#define STRAIGHT_RAD (180.f)     //直線時のRad
#define ROPE_RANGE (5.f)         //circle用の半径
#define ROPE_RECT_W (20)         //rect用の幅
#define ROPE_RECT_H (20)         //rect用の高さ
#define ROPE_OMEGA (1.0f)         //ここをいじることで1回で動くrect circleのposを変えれます
#define ROPE_LENGTH_MAX (45)     //ロープの最大長

class Input;
class Player;
class HitClass;
class MapCtl;
class ModelMgr;

struct Rect;
struct Circle;
struct INPUT_INFO;
struct L_STICK;
struct R_STICK;

enum SENSING_VALUE;

union KEY;

enum ROPE_DIR
{
	ROPE_DIR_NON,        //伸ばしていない状態
	ROPE_DIR_UPPER,      //斜め上に伸ばしている状態
	ROPE_DIR_STRAIGHT,   //まっすぐ伸ばしている状態
	ROPE_DIR_LOWER,      //斜め下に伸ばしている状態
};

enum ROPE_CURVE_DIR
{
	ROPE_DIR_CURVE_NON,  //伸ばしていない状態
	ROPE_DIR_CURVE_UP,   //斜め上に曲線を描きながら伸ばしている状態
	ROPE_DIR_CURVE_LOW   //斜め下に曲線を描きながら伸ばしている状態
};

class Rope : public Object
{
private:

	ModelMgr* _modelmgr;

	bool RopeTurnFlag;
	bool dirFlag;
	bool padFlag;
	bool tongueHitTurn;

	int    modelhandle;
	float  AnimTotalTime;
	float  AnimNowTime;
	int    AnimAttachIndex;

	int mentenanceNum;//モデルの描画のために仮で置いている調整用の値

	float range;
	float RopeAngle_Y;
	float RopeAngle_Z;

	void RopeInit(void);
	void DrawRopeRect(void);
	void SetRope(void);
	void DirChange(void);
	void CheckPad(void);
	//void DrawLineSet(Position2 startpos,Position2 endpos,int color);
	Position2 SetVec(void);
	float SetRopeRad(void);
	void SetRopeRadForDrawZ(void);

public:
	Rope();
	Rope(Player* player);
	~Rope();

	std::list<int> ropeinfo;
	std::list<int>::iterator itr = ropeinfo.begin();
	std::list<int>::iterator last = ropeinfo.end();

	Position2 _rope[ROPE_LENGTH_MAX];

	ROPE_STATE _state;
	ROPE_DIR _ropeDir;
	ROPE_CURVE_DIR _curveDir;
	SENSING_VALUE _minValue;

	KEY _key;
	KEY _lastkey;
	INPUT_INFO _inputinfo;
	Rect _RopeRect;
	Circle _RopeCircle;
	Position2 _HitPos;
	Position2 _vec;
	Position2 _tmpOffset;			//描画用ｵﾌｾｯﾄ値

	Player* _player;
	MapCtl* _mapctl;
	HitClass* _hit;

	void Updata(Input* input,Position2 offset);
	
	void(Rope::*_states)(Input* input);
	void Ready(Input* input);
	void SelectDir(Input* input);
	void Extending(Input* input);
	void Extended(Input* input);
	void Shrinking(Input* input);
	void GetClass(HitClass* h);		//クラスを受け取るための関数

	ROPE_DIR RopeDirSet(void);
	ROPE_DIR RopeDirSetPad(Input* input);
	ROPE_CURVE_DIR CurveRopeDirSet(void);
	Rect& GetRect(void);
	Circle& GetCircle(void);
	ROPE_STATE GetRopeState(void);
	Position2 GetReturnPos(void);
	Position2 GetRopeVec(void);

};
