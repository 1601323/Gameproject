#pragma once
#include <DxLib.h>
#include <math.h>
#include <cmath>

#define SCREEN_SIZE_X (800)	// 画面解像度：横
#define SCREEN_SIZE_Y (640)	// 画面解像度：縦

#define GRAVITY 0.9f
#define MAP_CHIP_SIZE_X (32)	// 1ﾁｯﾌﾟあたりの大きさ
#define MAP_CHIP_SIZE_Y (32)	// 1ﾁｯﾌﾟあたりの大きさ
#define MAP_SIZE_X (30)			//スクロール非対応マップの大きさ28
//#define MAP_SIZE_X (28)	    // ﾏｯﾌﾟのｻｲｽﾞ
#define MAP_SIZE_Y (25)			// ﾏｯﾌﾟのｻｲｽﾞ
#define STAGE_MAX (3)			//ステージの数（仮）
#define PLAYER_MAX (3)			//ﾌﾟﾚｲﾔｰの残機

//プレイヤーの状態について
enum CHAR_ST {
	ST_DEF,			//通常
	ST_MOVE,		//移動中
	ST_STOP,		//停止中
	ST_JUMP,		//ｼﾞｬﾝﾌﾟ
	ST_VANISH,		//ｽﾃﾙｽ状態
	ST_ROPE,		//ﾛｰﾌﾟ使用
	ST_WALL,		//壁登り
	ST_FEVER,		//ﾌｨｰﾊﾞｰ
	ST_DETH,		//死亡
	ST_CLEAR,		//ｸﾘｱ
	ST_MAX
};

//各種ﾁｯﾌﾟの種類[ｷﾞﾐｯｸとかで追加したいのあればここに書いてね] 
enum CHIP_TYPE {
	CHIP_BLANK,			//  0 空白
	CHIP_N_CLIMB_WALL,	//  1 登れない壁
	CHIP_CLIMB_WALL,	//  2 登れる壁
	CHIP_DOOR,			//  3 ドア
	CHIP_BUTTON_1,		//  4 ボタン
	CHIP_ROPE_ATTRACT,	//  5 ロープ引き寄せる
	CHIP_ROPE_FALL,		//  6 ロープﾌﾟ落とす
	CHIP_FREE_1,		//  7 自由
	CHIP_FREE_2,		//  8 自由
	CHIP_FREE_3,		//  9 自由
	CHIP_FREE_4,		// 10 自由
	CHIP_FREE_5,		// 11 自由
	CHIP_MAX
};

// ｷｬﾗｸﾀｰの向き
enum DIR {
	DIR_NON,			//状態なし
	DIR_DOWN,			//下
	DIR_UP,				//上
	DIR_LEFT,			//左
	DIR_RIGHT,			//右

	DIR_DETH = DIR_NON,	//死亡
	DIR_MAX
};

//ロープの状態
enum ROPE_STATE
{
	ST_ROPE_READY,       //準備中のstate
	ST_ROPE_SELECT,      //どの方向に伸ばすかを決めるstate
	ST_ROPE_EXTENDING,   //伸ばしてる間のstate
	ST_ROPE_EXTENDED,    //最大まで伸びきった時のstate
	ST_ROPE_SHRINKING,   //縮めている間のstate
};
//gimmickのタイプについて
enum GIMMICK_TYPE {
	GIM_NONE,
	GIM_DOOR,			//ドア
	GIM_BUTTON,			//ﾎﾞﾀﾝ
	GIM_ATTRACT,		//移動する床など
	GIM_FALL,			//落ちるギミックなど
};
//敵の種類について
enum ENEMY_TYPE {
	ENEMY_NONE,
	ENEMY_TURN,			//振り返る敵
	ENEMY_WARKING,		//歩き回る敵
	ENEMY_MIMICRY		//擬態する敵
};
//敵の状態について
enum ENEMY_STATE {
	EM_ST_NONE,		//何もない状態
	EM_ST_MOVE,		//通常の動きを行う
	EM_ST_ALERT,	//警戒度が上がっているとき
	EM_ST_DIS,		//ﾌﾟﾚｲﾔｰが見つかったとき
	EM_ST_RETURN,	//ﾌﾟﾚｲﾔｰが目的物を取って戻ってくるとき
	EM_ST_RE_ALERT,	//警戒度が上がっているとき（戻ってくるとき）
	EM_ST_RE_DIS,	//ﾌﾟﾚｲﾔｰが見つかったとき(戻ってくるとき)
	EM_ST_FEAR		//怯み状態
};
//アイテムの種類について
enum ITEM_TYPE {
	ITEM_FEVER,
	ITEM_MAX
};
//敵の警戒度について
enum ENEMY_ALERT {
	ALERT_LEVEL_1,
	ALERT_LEVEL_2,
	ALERT_LEVEL_3
};
//プレイヤーのアクション
enum PLAYER_ACTIONS{
	ACTION_WAIT,   //待機モーション
	ACTION_JUMP,   //飛ぶモーション
	ACTION_CLIMB,  //のぼるモーション
	ACTION_WALK,   //歩くモーション
	ACTION_MAX
};

struct EnemyServerData {

	EnemyServerData():plFoundFlag(false),dataSendFlag(false){}

	bool plFoundFlag;
	bool dataSendFlag;
	ENEMY_ALERT _level;
};
//キー入力について
struct KeyState {
	char state[256];
	char laststate[256];
};

//座標を表す
//とりあえず二次元で考える
struct Vector2 
{
	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) : x(inx), y(iny) {}
	float x, y;
};
//わかりやすくポジションに置き換える
typedef Vector2 Position2;

//こっちは三次元だぞ!
struct Vector3
{
	Vector3() :x(0), y(0) {}
	Vector3(float inx, float iny,float inz) : x(inx), y(iny), z(inz){}
	float x, y, z;
};
//ポジションに置き換える
typedef Vector3 Position3;

//場所の矩形について
struct Rect
{
	Position2 pos;

	int w, h;
	Rect() :pos(0, 0), w(0), h(0) {}

	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2 &inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh) {}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}

	Vector2 Center() {
		return pos;
	}
	//上下左右を確認する
	float Left()	{ return pos.x - (w / 2); }
	float Top()		{ return pos.y - (h / 2); }
	float Right()	{ return pos.x + (w / 2); }
	float Bottom()	{ return pos.y + (h / 2); }
	//四つ角をPositioin2で返す
	Position2 LeftTop()		 { return Position2(Left() ,Top());		}
	Position2 LeftBottom()	 { return Position2(Left() ,Bottom()-2);	}
	Position2 RightTop()	 { return Position2(Right(),Top());		}
	Position2 RightBottom()	 { return Position2(Right(),Bottom()-2);	}
	//あたり矩形を表示する
	void Draw(unsigned int color = 0xff00ffff) {
		DrawBox((int)Left(), (int)Top(), (int)Right(), (int)Bottom(), color, false);
	}
	void Draw(Position2 offset, unsigned int color = 0xff00ffff) {
		DrawBox((int)Left() - offset.x, (int)Top() - offset.y, (int)Right() - offset.x, (int)Bottom() - offset.y, color, false);
	}
};
//円について
struct Circle
{
	Position2 pos;
	float r;
	Circle() :pos(0, 0), r(0) {}
	Circle(float x, float y, float inr) :
		pos(x, y), r(inr) {}
	Circle(Position2 &inpos, float inr) :
		pos(inpos), r(inr) {}
	void SetCenter(float x,float y,float range) {
		pos.x = x;
		pos.y = y;
		r = range;
	}
	void SetCenter(Position2& inpos, float range) {
		pos.x = inpos.x;
		pos.y = inpos.y;
		r = range;
	}

	Vector2 Center() {
		return pos;
	}
	void Draw(unsigned int color = 0xff00ffff) {
		DrawCircle((int)pos.x,(int)pos.y,(int)r,color,false);
	}
	void Draw(Position2 offset, unsigned int color = 0xff00ffff) {
		DrawCircle((int)pos.x - offset.x, (int)pos.y - offset.y, (int)r, color, false);
	}
};
//敵のデータについて（あたり判定で使用）
struct ENEMY_DATA 
{
	ENEMY_DATA() :lookDir(DIR_NON), lookRange(), lookAngle(0) {}

	DIR lookDir;
	Circle lookRange;
	float lookAngle;
};
//ゴール時のデータについて
struct RESULT_DATA
{
	RESULT_DATA() :transFlag(false), midFlag(false),goalFlag(false), goalTime(0),life(PLAYER_MAX), foundCount(0) {}
	bool transFlag;			//画面遷移用フラグ
	bool midFlag;			//中間点に到着しているか
	bool goalFlag;			//ゴールに到達したかどうか
	int goalTime;
	int life;
	int foundCount;
};
inline
Position2 operator+(const Position2 &pos, const Position2 &pos2)
{
	return Position2(pos.x + pos2.x, pos.y + pos2.y);
}
inline
Position2 operator-(const Position2 &pos, const Position2 &pos2)
{
	return Position2(pos.x - pos2.x, pos.y - pos2.y);
}
