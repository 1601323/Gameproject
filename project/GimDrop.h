#pragma once
#include "Gimmick.h"
#include "Geometry.h"
#include "Input.h"
class Input;
class Rope;
class player;
class MapCtl;
class GimDrop :
	public Gimmick
{
private:
	HitClass* _hit;
	Rope& _rope;
	Player& _player;
	MapCtl* _map;
	GimmickState _state;
	Position2 _pos;
	FEVER_DATA _fd;

	bool dropFlag;	//落下フラグ
	float gravity;	//重力
	float velocity;	//速度
	//確認用
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	int count;		//表示秒数を設定
	void CheckDoMove();
	void Draw(Position2 offset);
	//ﾚｸﾄとの判定を見て動く方向を決める
	void MoveLeft();	
	void MoveRight();
	//ﾌｨｰﾊﾞｰｱｲﾃﾑとしてゲットする
	void GetItem();
public:
	GimDrop(Position2 pos,Rope& r,Player& p);
	~GimDrop();
	//Rect _gimRect;
	Rect& GetRect();
	GIMMICK_TYPE& GetType();
	void Updata(Input& input);
	void SendFeverData();
	
};

